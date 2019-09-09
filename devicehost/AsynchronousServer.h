#ifndef ASYNCHRONOUS_SERVER_H
#define ASYNCHRONOUS_SERVER_H

#include <vector>
#include "boost/shared_ptr.hpp"
#include "boost/unordered_map.hpp"
#include "DataStruct/FIFOList.h"
using FIFOList = NS(datastruct, 1)::FIFOList;
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
#include "Arithmetic/CVAlgo.h"
using CVAlgo = NS(algo, 1)::CVAlgo;

class AsynchronousServer final : public TransferModel
{
public:
	AsynchronousServer(
		boost::shared_ptr<CVAlgo> faceAlgo, const unsigned short port = 60532, FIFOList** fqueue = NULL);
	~AsynchronousServer(void);

protected:
	int initializeModel(MQContext& ctx) override;
	int deinitializeModel(MQContext& ctx) override;

private:
	int setNVR(
		const long long sequenceNo = 0,
		const char* request = NULL, const int requestBytes = 0, 
		const char* response = NULL, const int responseBytes = 0);
	int replySetNVR(
		const std::vector<DigitCamera>& cameras, 
		const char* NVRIp = NULL, const long long sequenceNo = 0, 
		const char* response = NULL, const int responseBytes = 0);
	int setCamera(
		const long long seqenceNo = 0, 
		const char* request = NULL, const int requestBytes = 0, 
		const char* response = NULL, const int responseBytes = 0);
	int setAutoSailingCheck(
		const long long sequenceNo = 0,
		const char* request = NULL, const int requestBytes = 0, 
		const char* response = NULL, const int responseBytes = 0);
	int setSailingStatus(
		const long long sequenceNo = 0,
		const char* request = NULL, const int requestBytes = 0, 
		const char* response = NULL, const int responseBytes = 0);
	int querySailingStatus(
		const long long sequenceNo = 0,
		const char* response = NULL, const int responseBytes = 0);
	int registerFace(
		const long long sequenceNo = 0,
		const char* request = NULL, const int requestBytes = 0, 
		const char* response = NULL, const int responseBytes = 0);
	int queryFaceInfos(
		const long long sequenceNo = 0,
		const char* request = NULL, const int requestBytes = 0, 
		const char* response = NULL, const int responseBytes = 0);
	int replyQueryFace(
		const std::vector<char*>& jpegFiles, const std::vector<int> jpegFileBytes, 
		const std::vector<long long> faceIDs,
		const int replyBytes = 0, const long long sequenceNo = 0,
		const char* response = NULL, const int responseBytes = 0);
	int captureCameraPicture(
		const long long sequenceNo = 0,
		const char* request = NULL, const int requestBytes = 0,
		const char* response = NULL, const int responseBytes = 0);
	//Return bytes used of response message.
	int getRequestMessageNotifyHandler(
		const char* request = NULL, const int requestBytes = 0,
		const char* response = NULL, const int responseBytes = 0);

private:
	BOOST_STATIC_CONSTANT(int, WORKER_THREAD_NUMBER = 5);
	std::vector<MQModelPtr> workerModels;
	boost::unordered_map<const std::string, HikvisionNVRDevicePtr> hikvisionNVRDevices;
	boost::unordered_map<const std::string, LivestreamPtr> livestreams;
	FIFOList** bgr24FrameQueue;
	boost::shared_ptr<CVAlgo> cvAlgoFacePtr;
};//class AsynchronousServer

#endif//ASYNCHRONOUS_SERVER_H

