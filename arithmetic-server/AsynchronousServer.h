#ifndef ASYNCHRONOUS_SERVER_H
#define ASYNCHRONOUS_SERVER_H

#include "boost/shared_ptr.hpp"
#include "MQModel/Transfer/TransferModel.h"
using TransferModel = NS(model, 1)::TransferModel;
using MQModel = NS(model, 1)::MQModel;
using MQModelPtr = boost::shared_ptr<MQModel>;
#include "Camera/Camera.h"
using Camera = NS(device, 1)::Camera;

class AsynchronousServer final : public TransferModel
{
public:
	AsynchronousServer(const unsigned short port = 60532);
	~AsynchronousServer(void);

public:
	void setClockUTCTime(const long long utc = 0);
	void setSailOrPort(const int status = 0);

protected:
	int initializeModel(MQContext& ctx) override;
	int deinitializeModel(MQContext& ctx) override;

private:
	int setNVR(
		const long long sequenceNo = 0,
		const char* request = NULL, const int requestBytes = 0, 
		const char* response = NULL, const int responseBytes = 0);
	int replySetNVR(
		const std::vector<Camera>& digitCameras, 
		const char* NVRIp = NULL, const long long sequenceNo = 0, 
		const char* response = NULL, const int responseBytes = 0);
	int setCamera(
		int& flag,
		const long long seqenceNo = 0, 
		const char* request = NULL, const int requestBytes = 0, 
		const char* response = NULL, const int responseBytes = 0);
	int setAutoCheck(
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
		const std::vector<long long> uuids,
		const int replyBytes = 0, const long long sequenceNo = 0,
		const char* response = NULL, const int responseBytes = 0);
	int captureCameraPicture(
		int& flag,
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
// 	boost::unordered_map<const std::string, HikvisionNVRDevicePtr> hikvisionNVRDevices;
// 	boost::unordered_map<const std::string, LivestreamPtr> livestreams;
};//class AsynchronousServer

#endif//ASYNCHRONOUS_SERVER_H

