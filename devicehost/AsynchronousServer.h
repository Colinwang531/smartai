#ifndef ASYNCHRONOUS_SERVER_H
#define ASYNCHRONOUS_SERVER_H

#include <vector>
#include "boost/shared_ptr.hpp"
#include "boost/unordered_map.hpp"
#include "MQModel/Transfer/TransferModel.h"
using TransferModel = NS(model, 1)::TransferModel;
using MQModel = NS(model, 1)::MQModel;
using MQModelPtr = boost::shared_ptr<MQModel>;
#include "Hikvision7xxxNVR.h"
using HikvisionDevice = NS(device, 1)::HikvisionDevice;
using HikvisionNVRDevicePtr = boost::shared_ptr<HikvisionDevice>;
#include "DigitCameraLivestream.h"
using Livestream = NS(stream, 1)::Livestream;
using LivestreamPtr = boost::shared_ptr<Livestream>;
#include "BGR24FrameCache.h"

class AsynchronousServer final : public TransferModel
{
public:
	AsynchronousServer(
		const unsigned short port = 60532, BGR24FrameCache* caches = NULL);
	~AsynchronousServer(void);

protected:
	int initializeModel(MQContext& ctx) override;
	int deinitializeModel(MQContext& ctx) override;

private:
	char* setNVR(
		const long long seqenceNo = 0, const char* request = NULL, const int requestBytes = 0);
 	char* setCamera(
		const long long seqenceNo = 0, const char* request = NULL, const int requestBytes = 0);
	char* replySetNVR(
		const std::vector<DigitCamera>& cameras, const char* NVRIp = NULL, const long long sequenceNo = 0);

	char* getRequestMessageNotifyHandler(
		const char* request = NULL, const int requestBytes = 0);

private:
	BOOST_STATIC_CONSTANT(int, WORKER_THREAD_NUMBER = 5);
	std::vector<MQModelPtr> workerModels;
	boost::unordered_map<const std::string, HikvisionNVRDevicePtr> hikvisionNVRDevices;
	boost::unordered_map<const std::string, LivestreamPtr> livestreams;
	BGR24FrameCache* bgr24FrameCaches;
};//class AsynchronousServer

#endif//ASYNCHRONOUS_SERVER_H

