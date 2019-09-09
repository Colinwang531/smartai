#include <io.h>
#include "boost/bind.hpp"
#include "boost/format.hpp"
#include "boost/filesystem/path.hpp"
#include "boost/filesystem/operations.hpp"
#include "boost/make_shared.hpp"
// #include "boost/property_tree/ptree.hpp"
// using PTREE = boost::property_tree::ptree;
// #include "boost/property_tree/xml_parser.hpp"
#ifdef _WINDOWS
#include "glog/log_severity.h"
#include "glog/logging.h"
#else
#include "glog/log_severity.h"
#include "glog/logging.h"
#endif//_WINDOWS
#include "error.h"
#include "MQModel/Worker/WorkerModel.h"
using WorkerModel = NS(model, 1)::WorkerModel;
#include "Arithmetic/CVAlgoFace.h"
using CVAlgoFace = NS(algo, 1)::CVAlgoFace;
#include "AsynchronousServer.h"

extern int sailingStatus;
extern int autoCheckSwitch;

AsynchronousServer::AsynchronousServer(
	boost::shared_ptr<CVAlgo> faceAlgo, const unsigned short port /* = 60532 */, FIFOList** fqueue /* = NULL */)
	: TransferModel(port), bgr24FrameQueue{ fqueue }, cvAlgoFacePtr{ faceAlgo }
{}

AsynchronousServer::~AsynchronousServer()
{}

int AsynchronousServer::initializeModel(MQContext& ctx)
{
	int status{ TransferModel::initializeModel(ctx) };

	if (ERR_OK == status)
	{
		for (int i = 0; i != WORKER_THREAD_NUMBER; ++i)
		{
			boost::shared_ptr<MQModel> workerModelPtr{
				boost::make_shared<WorkerModel>(
					"inproc://WorkerProcess", 
					boost::bind(&AsynchronousServer::getRequestMessageNotifyHandler, this, _1, _2, _3, _4)) };
			if (workerModelPtr && ERR_OK == workerModelPtr->start(ctx))
			{
				workerModels.push_back(workerModelPtr);
			}
		}
	}

	return status;
}

int AsynchronousServer::deinitializeModel(MQContext& ctx)
{
	for (std::vector<MQModelPtr>::iterator it = workerModels.begin(); it != workerModels.end(); ++it)
	{
		(*it)->stop(ctx);
	}

	return TransferModel::deinitializeModel(ctx);
}

int AsynchronousServer::setNVR(
	const long long sequenceNo /* = 0 */,
	const char* request /* = NULL */, const int requestBytes /* = 0 */, 
	const char* response /* = NULL */, const int responseBytes /* = 0 */)
{
	//Reply error message.
	int result{ 0 }, responseDataUsedBytes{ 20 };
	int* flag{ (int*)request };
	int* iplen{ (int*)(request + 4) };
	const std::string ipaddr(request + 8, *iplen);

	if (1 == *flag)
	{
		int pos{ 8 + *iplen };
		int* port{ (int*)(request + pos) };
		pos += 4;
		int* passwdlen{ (int*)(request + pos) };
		pos += 4;
		const std::string password(request + pos, *passwdlen);
		pos += *passwdlen;
		int* namelen{ (int*)(request + pos) };
		pos += 4;
		const std::string name(request + pos, *namelen);
		pos += *namelen;

		boost::shared_ptr<HikvisionDevice> devicePtr{ boost::make_shared<Hikvision7xxxNVR>() };
		if (devicePtr)
		{
			const int userID{ devicePtr->login(name.c_str(), password.c_str(), ipaddr.c_str(), *port) };

			if (-1 < userID)
			{
				result = 1;
				hikvisionNVRDevices.insert(std::make_pair(ipaddr, devicePtr));
				LOG(INFO) << "Login HIKVISION NVR " << ipaddr << " user ID " << userID;
				
				boost::shared_ptr<Hikvision7xxxNVR> nvr{
					boost::dynamic_pointer_cast<Hikvision7xxxNVR>(devicePtr) };
				std::vector<DigitCamera> cameras;
				nvr->getDigitCameras(userID, ipaddr.c_str(), cameras);
				responseDataUsedBytes = replySetNVR(cameras, ipaddr.c_str(), sequenceNo, response, responseBytes);
			}
			else
			{
				*((long long*)response) = sequenceNo;
				*((int*)(response + 8)) = 2;
				*((int*)(response + 12)) = 4;
				*((int*)(response + 16)) = result;
				*((int*)(response + 20)) = 0;
				*((int*)(response + 24)) = 0;
				responseDataUsedBytes = 28;
				LOG(WARNING) << "Login HIKVISION NVR " << ipaddr << " user ID " << userID;
			}
		}
	}
	else if (0 == *flag)
	{
		boost::unordered_map<const std::string, HikvisionNVRDevicePtr>::iterator it{ hikvisionNVRDevices.find(ipaddr) };
		if (hikvisionNVRDevices.end() != it)
		{
			//Remove all cameras.
			for (boost::unordered_map<const std::string, LivestreamPtr>::iterator it = livestreams.begin(); it != livestreams.end(); ++it)
			{
				it->second->close();
			}
			livestreams.clear();

			it->second->logout();
			hikvisionNVRDevices.erase(it);
			result = 1;
			LOG(INFO) << "Logout HIKVISION NVR " << ipaddr;
		}
		else
		{
			LOG(WARNING) << "Logout HIKVISION NVR failed " << ipaddr;
		}

		*((long long*)response) = sequenceNo;
		*((int*)(response + 8)) = 2;
		*((int*)(response + 12)) = 4;
		*((int*)(response + 16)) = result;
		*((int*)(response + 20)) = 0;
		*((int*)(response + 24)) = 0;
		responseDataUsedBytes = 28;
	}

	return responseDataUsedBytes;
}

int AsynchronousServer::replySetNVR(
	const std::vector<DigitCamera>& cameras, 
	const char* NVRIp /* = NULL */, const long long sequenceNo /* = 0 */, 
	const char* response /* = NULL */, const int responseBytes /* = 0 */)
{
	int pos{ 0 };
	const int messageID{ 2 }, result{ 1 }, NVRIpLen{ (int)strlen(NVRIp) }, cameraNumber{ (int)cameras.size() };
	memcpy_s((char*)response, 8, &sequenceNo, 8);
	pos += 8;
	memcpy_s((char*)response + pos, 4, &messageID, 4);
	pos += 8;
	memcpy_s((char*)response + pos, 4, &result, 4);
	pos += 4;
	memcpy_s((char*)response + pos, 4, &NVRIpLen, 4);
	pos += 4;
	memcpy_s((char*)response + pos, NVRIpLen, NVRIp, NVRIpLen);
	pos += NVRIpLen;
	memcpy_s((char*)response + pos, 4, &cameraNumber, 4);
	pos += 4;

	for (int i = 0; i != cameraNumber; ++i)
	{
		const int cameraIpLen{ (int)cameras[i].cameraIp.length() }, cameraIndex{ cameras[i].cameraIndex };
		memcpy_s((char*)response + pos, 4, &cameraIpLen, 4);
		memcpy_s((char*)response + pos + 4, cameraIpLen, cameras[i].cameraIp.c_str(), cameraIpLen);
		memcpy_s((char*)response + pos + 4 + cameraIpLen, 4, &cameraIndex, 4);
		pos += (8 + cameraIpLen);
	}
	const int packlen{ pos - 8 };
	memcpy_s((char*)response + 12, 4, &packlen, 4);

	return pos;
}

int AsynchronousServer::setCamera(
	const long long sequenceNo /* = 0 */,
	const char* request /* = NULL */, const int requestBytes /* = 0 */, 
	const char* response /* = NULL */, const int responseBytes /* = 0 */)
{
	int* NVRIpLen{ (int*)request };
	const std::string ipaddr(request + 4, *NVRIpLen);
	int* cameraIndex{ (int*)(request + 4 + *NVRIpLen) };
	int* algoFlag{ (int*)(request + 8 + *NVRIpLen) };
	int result{ 0 }, responseDataUsedBytes{ 20 };

	boost::unordered_map<const std::string, HikvisionNVRDevicePtr>::iterator it{ hikvisionNVRDevices.find(ipaddr) };
	if (hikvisionNVRDevices.end() != it)
	{
		const std::string NVRIpAddr{ (boost::format("%s_%d") % ipaddr % *cameraIndex).str() };

		if (0 < *algoFlag)
		{
			result = 1;
			boost::unordered_map<const std::string, LivestreamPtr>::iterator iter = livestreams.find(NVRIpAddr);

			if (livestreams.end() == iter)
			{
				boost::shared_ptr<Livestream> livestreamPtr{
					boost::make_shared<DigitCameraLivestream>(ipaddr, bgr24FrameQueue, *algoFlag) };
				if (livestreamPtr)
				{
					const int status{ livestreamPtr->open(it->second->getUserID(), *cameraIndex) };
					livestreams.insert(std::make_pair(NVRIpAddr, livestreamPtr));
					LOG(INFO) << "Add live stream " << NVRIpAddr << " and " << "ALGO mask "<< *algoFlag << ".";
				}
			} 
			else
			{
				boost::shared_ptr<DigitCameraLivestream> livestreamPtr{ 
					boost::dynamic_pointer_cast<DigitCameraLivestream>(iter->second) };
				if (livestreamPtr)
				{
					livestreamPtr->modifyAlgoMask(*algoFlag);
					LOG(INFO) << "Modify live stream " << NVRIpAddr << " ALGO mask" << *algoFlag << ".";
				}
			}
		}
		else
		{
			boost::unordered_map<const std::string, LivestreamPtr>::iterator it{
				livestreams.find((boost::format("%s_%d") % ipaddr % *cameraIndex).str()) };
			if (livestreams.end() != it)
			{
				it->second->close();
				livestreams.erase(it);
				result = 1;
				LOG(INFO) << "Remove live stream " << NVRIpAddr;
			}
		}
	}
	else
	{
		LOG(WARNING) << "Can not find NVR device " << ipaddr;
	}

	*((long long*)response) = sequenceNo;
	*((int*)(response + 8)) = 4;
	*((int*)(response + 12)) = 4;
	*((int*)(response + 16)) = result;
	return responseDataUsedBytes;
}

int AsynchronousServer::setAutoSailingCheck(
	const long long sequenceNo /* = 0 */,
	const char* request /* = NULL */, const int requestBytes /* = 0 */, 
	const char* response /* = NULL */, const int responseBytes /* = 0 */)
{
	int result{ 1 }, responseDataUsedBytes{ 20 };
	int* autoSailing{ (int*)(request) };
	autoCheckSwitch = *autoSailing;
	LOG(INFO) << "Set auto sailing control flag(0 : manual, 1 : auto) " << autoCheckSwitch;

	*((long long*)response) = sequenceNo;
	*((int*)(response + 8)) = 6;
	*((int*)(response + 12)) = 4;
	*((int*)(response + 16)) = result;

	return responseDataUsedBytes;
}

int AsynchronousServer::setSailingStatus(
	const long long sequenceNo /* = 0 */,
	const char* request /* = NULL */, const int requestBytes /* = 0 */, 
	const char* response /* = NULL */, const int responseBytes /* = 0 */)
{
	int result{ 1 }, responseDataUsedBytes{ 20 };
	int* currentSailingStatus{ (int*)(request) };
	sailingStatus = *currentSailingStatus;
	LOG(INFO) << "Set sailing status flag(0 : sailing, 1 : porting) " << sailingStatus;

	*((long long*)response) = sequenceNo;
	*((int*)(response + 8)) = 8;
	*((int*)(response + 12)) = 4;
	*((int*)(response + 16)) = 1;

	return responseDataUsedBytes;
}

int AsynchronousServer::querySailingStatus(
	const long long sequenceNo /* = 0 */,
	const char* response /* = NULL */, const int responseBytes /* = 0 */)
{
	int result{ 1 }, responseDataUsedBytes{ 20 };

	*((long long*)response) = sequenceNo;
	*((int*)(response + 8)) = 10;
	*((int*)(response + 12)) = 4;
	*((int*)(response + 16)) = sailingStatus;
	LOG(INFO) << "Reply sailing status flag(0 : sailing, 1 : porting) " << sailingStatus;

	return responseDataUsedBytes;
}

int AsynchronousServer::registerFace(
	const long long sequenceNo /* = 0 */,
	const char* request /* = NULL */, const int requestBytes /* = 0 */, 
	const char* response /* = NULL */, const int responseBytes /* = 0 */)
{
	int pos{ 0 };
	int* type{ (int*)request };
	pos += 4;
	long long* uuid{ (long long*)(request + pos) };
	pos += 8;
	int* namelen{ (int*)(request + pos) };
	pos += 4;
	const std::string name(request + pos, *namelen);
	pos += *namelen;
	int* imagelen{ (int*)(request + pos) };
	pos += 4;

	bool status{ false };
	std::string replyStr;
	boost::shared_ptr<CVAlgoFace> facePtr{ 
		boost::dynamic_pointer_cast<CVAlgoFace>(cvAlgoFacePtr) };
	if (facePtr)
	{
		const std::string executePath{
				boost::filesystem::initial_path<boost::filesystem::path>().string() };
		const std::string jpegFileName{
			(boost::format("%s\\Face\\%s_%d.jpg") % executePath % name % *uuid).str() };
		const std::string featureFileName{
			(boost::format("%s\\Face\\Feature.xml") % executePath).str() };
		const std::string personName{ (boost::format("%s_%d.Feature") % name % *uuid).str() };
		const std::string personID{ (boost::format("%s_%d.ID") % name % *uuid).str() };

		if (1 == *type)
		{
			try
			{
				FILE* jpegFile{ NULL };
				fopen_s(&jpegFile, jpegFileName.c_str(), "wb+");
				fwrite(request + pos, *imagelen, 1, jpegFile);
				fclose(jpegFile);

				StruFaceInfo featureInfo;
				status = facePtr->registerFace(featureInfo, jpegFileName.c_str());
				if (status)
				{
					status = facePtr->addFaceFeature(name.c_str(), *uuid, featureInfo.faceFeature);
					LOG(INFO) << "Add face feature of user " << name << " UUID " << *uuid << " status = " << status << ".";
				}
			}
			catch (std::exception*)
			{
				status = false;
			}
		} 
		else
		{
			if (!std::remove(jpegFileName.c_str()))
			{
				status = facePtr->removeFaceFeature(name.c_str(), *uuid);
				LOG(WARNING) << "Remove face feature of user " << name << " UUID " << *uuid << " status = " << status << ".";
			}
		}

		*((long long*)response) = sequenceNo;
		*((int*)(response + 8)) = 13;
		*((int*)(response + 12)) = 4;
		*((int*)(response + 16)) = status;
	}

	return pos;
}

int AsynchronousServer::queryFaceInfos(
	const long long sequenceNo /* = 0 */,
	const char* request /* = NULL */, const int requestBytes /* = 0 */, 
	const char* response /* = NULL */, const int responseBytes /* = 0 */)
{
	const std::string executePath{
			boost::filesystem::initial_path<boost::filesystem::path>().string() };
	std::vector<char*> jpegFiles;
	std::vector<int> jpegFileBytes;
	std::vector<long long> faceIDs;
	int pos = 0, status = ERR_BAD_OPERATE, replyDataBytes = 24;
	int* number{ (int*)request };
	pos += 4;

	for (int i = 0; i != *number; ++i)
	{
		long long* faceid{ (long long*)(request + pos) };
		pos += 8;
		int* namelen{ (int*)(request + pos) };
		pos += 4;
		const std::string fileName(request + pos, *namelen);
		const std::string jpegFileName{
			(boost::format("%s\\Face\\%s_%d.jpg") % executePath % fileName % *faceid).str() };
		pos += *namelen;

		try
		{
			char* jpegData = new char[1024 * 1024];
			FILE* jpegFile{ NULL };
			fopen_s(&jpegFile, jpegFileName.c_str(), "rb+");

			if (jpegFile)
			{
				const int readBytes{ (const int)_filelength(_fileno(jpegFile)) };
				fread(jpegData, 1024 * 1024, 1, jpegFile);
				fclose(jpegFile);
				jpegFiles.push_back(jpegData);
				jpegFileBytes.push_back(readBytes);
				faceIDs.push_back(*faceid);
				replyDataBytes += readBytes;
			}

			status = jpegFile ? ERR_OK : ERR_BAD_OPERATE;
		}
		catch (std::exception*)
		{
			for (int i = 0; i != jpegFileBytes.size(); ++i)
			{
				delete[] jpegFiles[i];
			}
			jpegFiles.clear();
			jpegFileBytes.clear();
		}

		LOG(INFO) << "Query face info of user " << jpegFileName << ".";
	}

	return replyQueryFace(jpegFiles, jpegFileBytes, faceIDs, replyDataBytes, sequenceNo, response, responseBytes);
}

int AsynchronousServer::replyQueryFace(
	const std::vector<char *>& jpegFiles, const std::vector<int> jpegFileBytes, 
	const std::vector<long long> faceIDs, const int replyBytes /* = 0 */, 
	const long long sequenceNo /* = 0 */,
	const char* response /* = NULL */, const int responseBytes /* = 0 */)
{
	int pos{ 24 };
	std::string replyMsg;
	const int jpegFileNumber = (const int)jpegFiles.size();
	const int replyDataBytes{ 0 < jpegFileNumber ? replyBytes : 24 };
	
	try
	{
		*((long long*)response) = sequenceNo;
		*((int*)(response + 8)) = 16;
		*((int*)(response + 12)) = 8;
		*((int*)(response + 16)) = 1;
		*((int*)(response + 20)) = jpegFileNumber;

		for (int i = 0; i != jpegFileNumber; ++i)
		{
			memcpy_s((char*)response + pos, 8, &faceIDs[i], 8);
			pos += 8;
			memcpy_s((char*)response + pos, 4, &jpegFileBytes[i], 4);
			pos += 4;
			memcpy_s((char*)response + pos, jpegFileBytes[i], jpegFiles[i], jpegFileBytes[i]);
			pos += jpegFileBytes[i];
		}
	}
	catch (std::exception*)
	{
		*((long long*)response) = sequenceNo;
		*((int*)(response + 8)) = 16;
		*((int*)(response + 12)) = 8;
		*((int*)(response + 16)) = 0;
		*((int*)(response + 20)) = 0;
	}

	return pos;
}

int AsynchronousServer::captureCameraPicture(
	const long long sequenceNo /* = 0 */,
	const char* request /* = NULL */, const int requestBytes /* = 0 */,
	const char* response /* = NULL */, const int responseBytes /* = 0 */)
{
	int pos = 0;
	int* iplen{ (int*)request };
	char* nvrip{ new char[*iplen + 1] };
	if (nvrip)
	{
		nvrip[*iplen] = 0;
		memcpy_s(nvrip, *iplen, request + 4, *iplen);
	}
	int* cameraIndex{ (int*)(request + 4 + *iplen) };

	const std::string cameraID{ (boost::format("%s_%d") % nvrip % *cameraIndex).str() };
	boost::unordered_map<const std::string, LivestreamPtr>::iterator it = livestreams.find(cameraID);
	if (livestreams.end() != it)
	{
		const int jpegPictureBytes = 1024 * 1024;
		char* jpegPicture{ new char[jpegPictureBytes] };

		if (jpegPicture)
		{
			boost::unordered_map<const std::string, HikvisionNVRDevicePtr>::iterator iter{ hikvisionNVRDevices.find(nvrip) };
			if (hikvisionNVRDevices.end() != iter)
			{
				const int capturePictureBytes{ it->second->capture(
					iter->second->getUserID(), *cameraIndex, jpegPicture, jpegPictureBytes) };
				*((long long*)response) = sequenceNo;
				*((int*)(response + 8)) = 20;
				*((int*)(response + 12)) = 12 + capturePictureBytes;
				*((int*)(response + 16)) = 1920;
				*((int*)(response + 20)) = 1080;
				*((int*)(response + 24)) = capturePictureBytes;
				memcpy_s((char*)(response + 28), capturePictureBytes, jpegPicture, capturePictureBytes);
				pos = 28 + capturePictureBytes;
			}
		}
	}

	LOG(INFO) << "Capture picture of living stream " << nvrip << "_" << *cameraIndex << " in bytes " << pos << ".";
	return pos;
}

int AsynchronousServer::getRequestMessageNotifyHandler(
	const char* request /* = NULL */, const int requestBytes /* = 0 */, 
	const char* response /* = NULL */, const int responseBytes /* = 0 */)
{
	int responseDataUsedBytes{ 0 };

	if (request && 0 < requestBytes)
	{
		long long* sequence{ (long long*)(request) };
		int* command{ (int*)(request + 8) };
		int* packlen{ (int*)(request + 12) };

		if (1 == *command)
		{
			responseDataUsedBytes = setNVR(*sequence, request + 16, *packlen, response, responseBytes);
		}
		else if (3 == *command)
		{
			responseDataUsedBytes = setCamera(*sequence, request + 16, *packlen, response, responseBytes);
		}
		else if (5 == *command)
		{
			responseDataUsedBytes = setAutoSailingCheck(*sequence, request + 16, *packlen, response, responseBytes);
		}
		else if (7 == *command)
		{
			responseDataUsedBytes = setSailingStatus(*sequence, request + 16, *packlen, response, responseBytes);
		}
		else if (9 == *command)
		{
			responseDataUsedBytes = querySailingStatus(*sequence, response, responseBytes);
		}
		else if (12 == *command)
		{
			responseDataUsedBytes = registerFace(*sequence, request + 16, *packlen, response, responseBytes);
		}
		else if (14 == *command)
		{
			responseDataUsedBytes = queryFaceInfos(*sequence, request + 16, *packlen, response, responseBytes);
		}
		else if (19 == *command)
		{
			responseDataUsedBytes = captureCameraPicture(*sequence, request + 16, *packlen, response, responseBytes);
		}
	}

	return responseDataUsedBytes;
}
