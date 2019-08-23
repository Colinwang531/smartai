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
#include "AsynchronousServer.h"

AsynchronousServer::AsynchronousServer(
	const unsigned short port /* = 60532 */, BGR24FrameCache* caches /* = NULL */)
	: TransferModel(port), bgr24FrameCaches{ caches }
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
					boost::bind(&AsynchronousServer::getRequestMessageNotifyHandler, this, _1, _2)) };
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

char* AsynchronousServer::setNVR(
	const long long seqenceNo /* = 0 */, const char* request /* = NULL */, const int requestBytes /* = 0 */)
{
	char* reply = NULL;
	//Reply error message.
	int result{ 0 };
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
				reply = replySetNVR(cameras, ipaddr.c_str(), seqenceNo);
			}
			else
			{
				reply = new char[20];
				*((long long*)reply) = seqenceNo;
				*((int*)(reply + 8)) = 2;
				*((int*)(reply + 12)) = 4;
				*((int*)(reply + 16)) = result;
				LOG(WARNING) << "Login HIKVISION NVR " << ipaddr << " user ID " << userID;
			}
		}
	}
	else if (0 == *flag)
	{
		boost::unordered_map<const std::string, HikvisionNVRDevicePtr>::iterator it{ hikvisionNVRDevices.find(ipaddr) };
		if (hikvisionNVRDevices.end() != it)
		{
			it->second->logout();
			hikvisionNVRDevices.erase(it);
			result = 1;
			LOG(INFO) << "Logout HIKVISION NVR " << ipaddr;
		}
		else
		{
			LOG(INFO) << "Logout HIKVISION NVR failed " << ipaddr;
		}

		reply = new char[20];
		*((long long*)reply) = seqenceNo;
		*((int*)(reply + 8)) = 2;
		*((int*)(reply + 12)) = 4;
		*((int*)(reply + 16)) = result;
	}

	return reply;
}

char* AsynchronousServer::setCamera(
	const long long seqenceNo /* = 0 */, const char* request /* = NULL */, const int requestBytes /* = 0 */)
{
	int* NVRIpLen{ (int*)request };
	const std::string ipaddr(request + 4, *NVRIpLen);
	int* cameraIndex{ (int*)(request + 4 + *NVRIpLen) };
	int* algoFlag{ (int*)(request + 8 + *NVRIpLen) };
	int result{ 0 };

	boost::unordered_map<const std::string, HikvisionNVRDevicePtr>::iterator it{ hikvisionNVRDevices.find(ipaddr) };
	if (hikvisionNVRDevices.end() != it)
	{
		const std::string NVRIpAddr{ (boost::format("%s_%d") % ipaddr % *cameraIndex).str() };

		if (0 < *algoFlag)
		{
			boost::shared_ptr<Livestream> livestreamPtr{
				boost::make_shared<DigitCameraLivestream>(ipaddr, bgr24FrameCaches, *algoFlag) };
			if (livestreamPtr)
			{
				result = 1;
				const int status{ livestreamPtr->open(it->second->getUserID(), *cameraIndex) };
				livestreams.insert(std::make_pair(NVRIpAddr, livestreamPtr));
				LOG(INFO) << "Add live stream " << NVRIpAddr << " : " << status;
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

	char* reply = new char[20];
	*((long long*)reply) = seqenceNo;
	*((int*)(reply + 8)) = 3;
	*((int*)(reply + 12)) = 4;
	*((int*)(reply + 16)) = result;

	return reply;
}

char* AsynchronousServer::replySetNVR(
	const std::vector<DigitCamera>& cameras, const char* NVRIp /* = NULL */, const long long sequenceNo /* = 0 */)
{
	int pos{ 0 };
	const int messageID{ 2 }, result{ 1 }, NVRIpLen{ (int)strlen(NVRIp) }, cameraNumber{ (int)cameras.size() };
	char* msg = new char[1024 * 1024];
	memcpy_s(msg, 8, &sequenceNo, 8);
	pos += 8;
	memcpy_s(msg+ pos, 4, &messageID, 4);
	pos += 8;
	memcpy_s(msg + pos, 4, &result, 4);
	pos += 4;
	memcpy_s(msg + pos, 4, &NVRIpLen, 4);
	pos += 4;
	memcpy_s(msg + pos, NVRIpLen, NVRIp, NVRIpLen);
	pos += NVRIpLen;
	memcpy_s(msg + pos, 4, &cameraNumber, 4);
	pos += 4;

	for (int i = 0; i != cameraNumber; ++i)
	{
		const int cameraIpLen{ (int)cameras[i].cameraIp.length() }, cameraIndex{ cameras[i].cameraIndex };
		memcpy_s(msg + pos, 4, &cameraIpLen, 4);
		memcpy_s(msg + pos + 4, cameraIpLen, cameras[i].cameraIp.c_str(), cameraIpLen);
		memcpy_s(msg + pos + 4 + cameraIpLen, 4, &cameraIndex, 4);
		pos += (8 + cameraIpLen);
	}
	memcpy_s(msg + 12, 4, &pos, 4);

	char* replyMsg = new char[pos];
	memcpy_s(replyMsg, pos, msg, pos);
//	TransferModel::sendReply(replyMsg, pos);
	delete[] msg;
//	delete[] replyMsg;

	return replyMsg;
}

char* AsynchronousServer::getRequestMessageNotifyHandler(
	const char* request /* = NULL */, const int requestBytes /* = 0 */)
{
	char* replyMsg = NULL;

	if (request && 0 < requestBytes)
	{
		long long* sequence{ (long long*)(request) };
		int* command{ (int*)(request + 8) };
		int* packlen{ (int*)(request + 12) };

		if (1 == *command)
		{
			replyMsg = setNVR(*sequence, request + 16, *packlen);
		}
		else if (3 == *command)
		{
			setCamera(*sequence, request + 16, *packlen);
		}
	}

	return replyMsg;
}
