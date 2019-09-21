//
//		Copyright :					@2018, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2019-07-19
//		Description :				CV算法基类
//
//		History:						Author										Date													Description
//											王科威										2019-08-26									创建
//

#ifndef CV_ALGO_H
#define CV_ALGO_H

#include <vector>
#include <windows.h>
#include "boost/function.hpp"
#include "boost/shared_ptr.hpp"
#include "boost/thread/mutex.hpp"
#include "boost/unordered_map.hpp"
#include "Stream/Livestream.h"
using LivestreamPtr = boost::shared_ptr<NS(stream, 1)::Livestream>;
using LivestreamGroup = boost::unordered_map<const std::string, LivestreamPtr>;

NS_BEGIN(algo, 1)

typedef enum tagAlgoType_t
{
	ALGO_HELMET = 0,
	ALGO_PHONE,
	ALGO_SLEEP,
	ALGO_FIGHT,
	ALGO_FACE,
	ALGO_NONE
}AlgoType;

typedef struct tagFaceDetect_t
{
	int similarity;
	int imageBytes;
	char* imageData;
	long long faceID;
}FaceDetect;

typedef struct tagDetectNotify_t
{
	int type;
	int status;
	int x;
	int y;
	int w;
	int h;
	FaceDetect face;
}DetectNotify;

typedef boost::function<void(void*, std::vector<DetectNotify>)> CaptureAlarmNotifyHandler;

class CVAlgo
{
public:
	CVAlgo(CaptureAlarmNotifyHandler handler = NULL);
	virtual ~CVAlgo(void);

public:
	bool initialize(
		const char* configFilePath = NULL, const float detectThreshold = 0.0f, const float trackThreshold = 0.0f);
	int addLivestream(const std::string streamID, LivestreamPtr livestreamPtr);
	int removeLivestream(const std::string streamID);

protected:
	virtual bool initializeWithParameter(void* parameter = NULL) = 0;
	virtual void algorithmWorkerProcess(void) = 0;

private:
	static DWORD WINAPI algorithmProcessThread(void* ctx = NULL);

protected:
	CaptureAlarmNotifyHandler captureAlarmNotifyHandler;
	BOOST_STATIC_CONSTANT(unsigned short, IMAGE_WIDTH = 1920);
	BOOST_STATIC_CONSTANT(unsigned short, IMAGE_HEIGHT = 1080);
	BOOST_STATIC_CONSTANT(unsigned short, CHANNEL_NUMBER = 3);

protected:
	boost::mutex livestreamMtx;
	LivestreamGroup livestreamGroup;
	static DWORD enableAlgorithmCount;
};//class CVAlgo

NS_END

#endif//CV_ALGO_H
