#ifndef DEVICE_HOST_SERVER_H
#define DEVICE_HOST_SERVER_H

#include "boost/shared_ptr.hpp"
#include "boost/unordered_map.hpp"
#include "MQModel/Router/RouterModel.h"
using RouterModel = NS(model, 1)::RouterModel;
#include "Hikvision7xxxNVR.h"
using HikvisionDevice = NS(device, 1)::HikvisionDevice;
using HikvisionNVRDevicePtr = boost::shared_ptr<HikvisionDevice>;
#include "DigitCameraLivestream.h"
using Livestream = NS(stream, 1)::Livestream;
using LivestreamPtr = boost::shared_ptr<Livestream>;
#include "BGR24FrameCache.h"

class DeviceHostServer final : public RouterModel
{
public:
	DeviceHostServer(
		const unsigned short port = 60532, BGR24FrameCache* caches = NULL);
	~DeviceHostServer(void);

protected:
	void gotMessageData(const char* data = NULL, const int dataBytes = 0) override;

private:
	int setNVRDevice(const char* data = NULL, const int dataBytes = 0);
	int setCamera(const char* data = NULL, const int dataBytes = 0);
	void digitCameraParametersNotifyHandler(
		const std::vector<DigitCameraParameters>& parameters, const char* NVRIp = NULL);

private:
	boost::unordered_map<const std::string, HikvisionNVRDevicePtr> hikvisionNVRDevices;
	boost::unordered_map<const std::string, LivestreamPtr> livestreams;
	BGR24FrameCache* bgr24FrameCaches;
};//class DeviceHostServer

#endif//DEVICE_HOST_SERVER_H

