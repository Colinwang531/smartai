#include <io.h>
#include "boost/bind.hpp"
#include "boost/format.hpp"
#include "boost/make_shared.hpp"
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
using CVAlgo = NS(algo, 1)::CVAlgo;
using CVAlgoFace = NS(algo, 1)::CVAlgoFace;
#include "devicehost.h"
#include "AsynchronousServer.h"

AsynchronousServer::AsynchronousServer(const unsigned short port /* = 60532 */)
	: TransferModel(port)
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

		std::vector<Camera> digitCameras;
		result = createNewNVRDevice(ipaddr, *port, name, password, digitCameras);

		if (ERR_OK == result)
		{
			responseDataUsedBytes = replySetNVR(digitCameras, ipaddr.c_str(), sequenceNo, response, responseBytes);
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
		}
	}
	else if (0 == *flag)
	{
		result = destroyNVRDevice(ipaddr);

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
	const std::vector<Camera>& digitCameras, 
	const char* NVRIp /* = NULL */, const long long sequenceNo /* = 0 */, 
	const char* response /* = NULL */, const int responseBytes /* = 0 */)
{
	int pos{ 0 };
	const int messageID{ 2 }, result{ 1 }, NVRIpLen{ (int)strlen(NVRIp) }, cameraNumber{ (int)digitCameras.size() };
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
		const int cameraIpLen{ (int)digitCameras[i].getCameraIP().length() };
		const unsigned long long cameraIndex{ digitCameras[i].getCameraIndex() };
		memcpy_s((char*)response + pos, 4, &cameraIpLen, 4);
		memcpy_s((char*)response + pos + 4, cameraIpLen, digitCameras[i].getCameraIP().c_str(), cameraIpLen);
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
	int* arithmeticAbilities{ (int*)(request + 8 + *NVRIpLen) };
	int result{ 0 }, responseDataUsedBytes{ 20 };

	if (0 < *arithmeticAbilities)
	{
		result = createNewDigitCamera(ipaddr, *cameraIndex, *arithmeticAbilities);
	}
	else
	{
		result = destroyDigitCamera(ipaddr, *cameraIndex);
	}

	*((long long*)response) = sequenceNo;
	*((int*)(response + 8)) = 4;
	*((int*)(response + 12)) = 4;
	*((int*)(response + 16)) = result;

	return responseDataUsedBytes;
}

int AsynchronousServer::setAutoCheck(
	const long long sequenceNo /* = 0 */,
	const char* request /* = NULL */, const int requestBytes /* = 0 */, 
	const char* response /* = NULL */, const int responseBytes /* = 0 */)
{
	int responseDataUsedBytes{ 20 };
	int* autoSailing{ (int*)(request) };

	*((long long*)response) = sequenceNo;
	*((int*)(response + 8)) = 6;
	*((int*)(response + 12)) = 4;
	*((int*)(response + 16)) = setAutoCheckSailOrPort(*autoSailing);

	return responseDataUsedBytes;
}

int AsynchronousServer::setSailingStatus(
	const long long sequenceNo /* = 0 */,
	const char* request /* = NULL */, const int requestBytes /* = 0 */, 
	const char* response /* = NULL */, const int responseBytes /* = 0 */)
{
	int responseDataUsedBytes{ 20 };
	int* sailingStatus{ (int*)(request) };

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
	int responseDataUsedBytes{ 20 };

	*((long long*)response) = sequenceNo;
	*((int*)(response + 8)) = 10;
	*((int*)(response + 12)) = 4;
	*((int*)(response + 16)) = getSailingStatus();

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

	*((long long*)response) = sequenceNo;
	*((int*)(response + 8)) = 13;
	*((int*)(response + 12)) = 4;
	*((int*)(response + 16)) = createNewFacePicture(request + pos, *imagelen, name.c_str(), *uuid);

	return pos;
}

int AsynchronousServer::queryFaceInfos(
	const long long sequenceNo /* = 0 */,
	const char* request /* = NULL */, const int requestBytes /* = 0 */, 
	const char* response /* = NULL */, const int responseBytes /* = 0 */)
{
	std::vector<char*> jpegFiles;
	std::vector<int> jpegFilesBytes;
	std::vector<long long> uuids;
	int pos = 0, status = ERR_BAD_OPERATE, replyDataBytes = 24;
	int* number{ (int*)request };
	pos += 4;
	
	for (int i = 0; i != *number; ++i)
	{
		long long* uuid{ (long long*)(request + pos) };
		pos += 8;
		int* namelen{ (int*)(request + pos) };
		pos += 4;
		const std::string fileName(request + pos, *namelen);
		pos += *namelen;

		char* jpegImageData = new(std::nothrow) char[512 * 1024];
		if (jpegImageData)
		{
			int jpegImageBytes{ 0 };
			const int queryFaceResult{ queryFacePicture(*uuid, jpegImageData, jpegImageBytes) };

			if (0 < jpegImageBytes)
			{
				jpegFiles.push_back(jpegImageData);
				jpegFilesBytes.push_back(jpegImageBytes);
				uuids.push_back(*uuid);
				replyDataBytes += (12 + jpegImageBytes);
			}
		}
	}

	return replyQueryFace(jpegFiles, jpegFilesBytes, uuids, replyDataBytes, sequenceNo, response, responseBytes);
}

int AsynchronousServer::replyQueryFace(
	const std::vector<char *>& jpegFiles, const std::vector<int> jpegFileBytes, 
	const std::vector<long long> uuids, const int replyBytes /* = 0 */, 
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
			memcpy_s((char*)response + pos, 8, &uuids[i], 8);
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
 	int* NVRIpLen{ (int*)request };
	const std::string NVRIpAddr(request + 4, *NVRIpLen);
 	int* cameraIndex{ (int*)(request + 4 + *NVRIpLen) };

	MediaImagePtr captureImagePtr{ 
		captureDigitCameraLivePicture(NVRIpAddr, *cameraIndex) };
	if (captureImagePtr)
	{
		const int captureImageBytes{ (const int)captureImagePtr->getImageBytes() };
		*((long long*)response) = sequenceNo;
		*((int*)(response + 8)) = 20;
		*((int*)(response + 12)) = 12 + captureImageBytes;
		*((int*)(response + 16)) = 1920;
		*((int*)(response + 20)) = 1080;
		*((int*)(response + 24)) = captureImageBytes;
		memcpy_s((char*)(response + 28), captureImageBytes, captureImagePtr->getImage(), captureImageBytes);
		pos = 28 + captureImageBytes;
	}

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
			responseDataUsedBytes = setAutoCheck(*sequence, request + 16, *packlen, response, responseBytes);
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
