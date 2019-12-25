// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Abstract base class of AI arithmetic.
//

#ifndef CV_ALGO_H
#define CV_ALGO_H

#include "boost/function.hpp"
#include "boost/shared_ptr.hpp"
#include "boost/thread/condition.hpp"
#include "boost/thread/thread_pool.hpp"
#include "boost/winapi/time.hpp"
#include "DefGlobalVar.h"
#include "MediaData/MediaData.h"
using MediaData = NS(media, 1)::MediaData;
#include "DataStruct/FIFOQueue.h"
using MediaDataPtr = boost::shared_ptr<MediaData>;
using FIFOQueue = NS(datastruct, 1)::FIFOQueue<MediaDataPtr>;

NS_BEGIN(algo, 1)

typedef enum class tagAlarmType_t
{
	ALARM_TYPE_NONE = 0,
	ALARM_TYPE_HELMET,
	ALARM_TYPE_PHONE,
	ALARM_TYPE_SLEEP,
	ALARM_TYPE_FIGHT,
	ALARM_TYPE_FACE,
}AlarmType;

typedef struct tagFaceInfo_t
{
	AlarmType type;
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
	int faceID;
	float similarity;
	unsigned char* faceImage;
	unsigned long long imageBytes;
}AlarmInfo;

typedef boost::function<void(const std::vector<AlarmInfo>, const unsigned char*, const unsigned long long)> PostDetectAlarmInfoCallback;
// typedef boost::function<void(MediaImagePtr, std::vector<FaceInfo>)> CaptureFaceInfoHandler;

class CVAlgo
{
public:
	CVAlgo(void);
	virtual ~CVAlgo(void);

public:
	int initialize(void);
	void deinitialize(void);
	inline void setArithmeticID(const int id = 0)
	{
		arithmeticID = id;
	}
	inline void setPostDetectAlarmInfoCallback(PostDetectAlarmInfoCallback callback = NULL)
	{
		postDetectAlarmInfoCallback = callback;
	}
	int inputImageData(const unsigned char* imageData = NULL, const int imageBytes = 0);

protected:
	virtual int initializeArithmetic(void) = 0;
	virtual int deinitializeArithmetic(void) = 0;
	virtual void arithmeticWorkerProcess(void) = 0;
	void getArithmeticInitParameter(
		StruInitParams& parameters, const AlarmType alarmType = AlarmType::ALARM_TYPE_NONE);

private:
	void arithmeticProcessThread(void);

protected:
// 	CaptureAlarmInfoHandler captureAlarmInfoHandler;
// 	CaptureFaceInfoHandler capturefaceInfoHandler;
	BOOST_STATIC_CONSTANT(unsigned short, IMAGE_WIDTH = 1920);
	BOOST_STATIC_CONSTANT(unsigned short, IMAGE_HEIGHT = 1080);
	BOOST_STATIC_CONSTANT(unsigned short, CHANNEL_NUMBER = 3);

protected:
	FIFOQueue BGR24ImageQueue;
	bool stopped;
	//Guarantee work thread exited safely. 
	boost::mutex mtx;
	boost::condition condition;
	PostDetectAlarmInfoCallback postDetectAlarmInfoCallback;
	static boost::thread_group threadGroup;
	unsigned long long totalFrameNumber;

private:
	int arithmeticID;
};//class CVAlgo

NS_END

#endif//CV_ALGO_H
