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
#include "DeviceHostServer.h"

DeviceHostServer::DeviceHostServer(
	const unsigned short port /* = 60532 */, BGR24FrameCache* caches /* = NULL */)
	: TransferModel(port), bgr24FrameCaches{ caches }
{}

DeviceHostServer::~DeviceHostServer()
{}

void DeviceHostServer::gotMessageData(const char* request /* = NULL */, const int requestBytes /* = 0 */)
{
	if (request && 0 < requestBytes)
	{
		int* type{ (int*)(request) };
		int* packlen{ (int*)(request + 4) };
		if (1 == *type)
		{
			setNVRDevice(request + 8, *packlen);
		}
		else if (3 == *type)
		{
			setCamera(request + 8, *packlen);
		}
	}
}

int DeviceHostServer::setNVRDevice(const char* data /* = NULL */, const int dataBytes /* = 0 */)
{
	//Reply error message.
	int result{ 0 };
	int* flag{ (int*)data };
	int* iplen{ (int*)(data + 4) };
	const std::string ipaddr(data + 8, *iplen);

	if (1 == *flag)
	{
		int pos{ 8 + *iplen };
		int* port{ (int*)(data + pos) };
		int* passwdlen{ (int*)(data + pos + 4) };
		const std::string password(data + pos + 8, *passwdlen);
		pos += (8 + *passwdlen);
		int* namelen{ (int*)(data + pos) };
		const std::string name(data + pos + 4, *namelen);

		boost::shared_ptr<HikvisionDevice> devicePtr{
			boost::make_shared<Hikvision7xxxNVR>(
				boost::bind(&DeviceHostServer::digitCameraParametersNotifyHandler, this, _1, _2)) };
		if (devicePtr)
		{
			const int userID{ devicePtr->login(name.c_str(), password.c_str(), ipaddr.c_str(), *port) };

			if (-1 < userID)
			{
				result = 1;
				hikvisionNVRDevices.insert(std::make_pair(ipaddr, devicePtr));
				LOG(INFO) << "Login HIKVISION NVR " << ipaddr << " user ID " << userID;
			}
			else
			{
				char replyMsg[12]{};
				*((int*)replyMsg) = 2;
				*((int*)(replyMsg + 4)) = 4;
				*((int*)(replyMsg + 8)) = result;
				TransferModel::sendReply(replyMsg, 12);

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

		char replyMsg[12]{};
		*((int*)replyMsg) = 2;
		*((int*)(replyMsg + 4)) = 4;
		*((int*)(replyMsg + 8)) = result;
		TransferModel::sendReply(replyMsg, 12);
	}

	return ERR_OK;
}

int DeviceHostServer::setCamera(const char* data /* = NULL */, const int dataBytes /* = 0 */)
{
	int* NVRIpLen{ (int*)data };
	const std::string ipaddr(data + 4, *NVRIpLen);
	int* cameraIndex{ (int*)(data + 4 + *NVRIpLen) };
	int* algoFlag{ (int*)(data + 8 + *NVRIpLen) };
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
				LOG(INFO) << "Remove live stream " << NVRIpAddr;
			}
		}
	}
	else
	{
		LOG(WARNING) << "Can not find NVR device " << ipaddr;
	}

	char* replyMsg = new char[12];
	*((int*)replyMsg) = 4;
	*((int*)(replyMsg + 4)) = 4;
	*((int*)(replyMsg + 8)) = result;
	TransferModel::sendReply(replyMsg, 12);

	return ERR_OK;
}

void DeviceHostServer::digitCameraParametersNotifyHandler(
	const std::vector<DigitCameraParameters>& parameters, const char* NVRIp /* = NULL */)
{
	int pos{ 0 };
	const int messageID{ 2 }, result{ 1 }, NVRIpLen{ (int)strlen(NVRIp) }, cameraNumber{ (int)parameters.size() };
	char* msg = new char[1024 * 1024];
	memcpy_s(msg, 4, &messageID, 4);
	memcpy_s(msg + 8, 4, &result, 4);
	memcpy_s(msg + 12, 4, &NVRIpLen, 4);
	memcpy_s(msg + 16, NVRIpLen, NVRIp, NVRIpLen);
	memcpy_s(msg + 16 + NVRIpLen, 4, &cameraNumber, 4);
	pos += (16 + NVRIpLen);
	memcpy_s(msg + pos, 4, &cameraNumber, 4);
	pos += 4;

	for (int i = 0; i != cameraNumber; ++i)
	{
		const int cameraIpLen{ (int)parameters[i].cameraIp.length() }, cameraIndex{ parameters[i].cameraIndex };
		memcpy_s(msg + pos, 4, &cameraIpLen, 4);
		memcpy_s(msg + pos + 4, cameraIpLen, parameters[i].cameraIp.c_str(), cameraIpLen);
		memcpy_s(msg + pos + 4 + cameraIpLen, 4, &cameraIndex, 4);
		pos += (8 + cameraIpLen);
	}
	memcpy_s(msg + 4, 4, &pos, 4);

	char* replyMsg = new char[pos];
	memcpy_s(replyMsg, pos, msg, pos);
	TransferModel::sendReply(replyMsg, pos);
	delete[] msg;
//	delete[] replyMsg;
}
