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
#include "MediaFrame/MediaFrame.h"
using MediaImage = NS(frame, 1)::MediaImage;
#include "DataStruct/FIFOQueue.h"
using MediaImagePtr = boost::shared_ptr<MediaImage>;
using FIFOQueue = NS(datastruct, 1)::FIFOQueue<MediaImagePtr>;

NS_BEGIN(algo, 1)

typedef enum class tagAlarmType_t
{
	ALARM_TYPE_HELMET = 0,
	ALARM_TYPE_PHONE,
	ALARM_TYPE_SLEEP,
	ALARM_TYPE_FIGHT,
	ALARM_TYPE_FACE,
	ALARM_TYPE_NONE
}AlarmType;

typedef struct tagFaceInfo_t
{
	int similarity;
	int imageBytes;
	char* imageData;
	long long faceID;
}FaceInfo;

typedef struct tagAlarmInfo_t
{
	AlarmType type;
	int status;
	int x;
	int y;
	int w;
	int h;
	FaceInfo faceInfo;
}AlarmInfo;

typedef boost::function<void(MediaImagePtr, std::vector<AlarmInfo>)> CaptureAlarmInfoHandler;

class CVAlgo
{
public:
	CVAlgo(CaptureAlarmInfoHandler handler = NULL);
	virtual ~CVAlgo(void);

public:
	virtual int initialize(
		const char* configFilePath = NULL, const float detectThreshold = 0.0f, const float trackThreshold = 0.0f);
//	virtual void deinitialize(void) = 0;
	int tryInputMediaImage(MediaImagePtr image);

protected:
	virtual int initializeWithParameter(const char* configFilePath = NULL, void* parameter = NULL) = 0;
	virtual void arithmeticWorkerProcess(void) = 0;

private:
	static DWORD WINAPI arithmeticProcessThread(void* ctx = NULL);

protected:
	CaptureAlarmInfoHandler captureAlarmInfoHandler;
	BOOST_STATIC_CONSTANT(unsigned short, IMAGE_WIDTH = 1920);
	BOOST_STATIC_CONSTANT(unsigned short, IMAGE_HEIGHT = 1080);
	BOOST_STATIC_CONSTANT(unsigned short, CHANNEL_NUMBER = 3);

protected:
//	boost::mutex livestreamMtx;
	FIFOQueue BGR24ImageQueue;
//	static DWORD enableAlgorithmCount;
	bool arithmeticProcessing;
};//class CVAlgo

NS_END

#endif//CV_ALGO_H
