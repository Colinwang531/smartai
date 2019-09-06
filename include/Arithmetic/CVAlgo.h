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
#include "boost/function.hpp"
#include "DataStruct/FIFOList.h"
using FIFOList = NS(datastruct, 1)::FIFOList;

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

typedef boost::function<void(void*, std::vector<DetectNotify>)> CVAlgoDetectNotifyHandler;

class CVAlgo
{
public:
	CVAlgo(
		int& enable, FIFOList* fqueue = NULL, CVAlgoDetectNotifyHandler handler = NULL);
	virtual ~CVAlgo(void);

public:
	bool initialize(
		const char* path = NULL, const float detectThreshold = 0.0f, const float trackThreshold = 0.0f,
		const int width = 1920, const int height = 1080, const int channel = 3);

protected:
	virtual bool initializeWithParameter(void* parameter = NULL) = 0;
	virtual void mainWorkerProcess(void) = 0;
	virtual void subWorkerProcess(void);

private:
	static unsigned int __stdcall mainWorkerThread(void* ctx = NULL);
	static unsigned int __stdcall subWorkerThread(void* ctx = NULL);

protected:
	int& enableAlgoFlag;
	FIFOList* frameQueue;
	CVAlgoDetectNotifyHandler cvAlgoDetectNotifyHandler;
	int imageWidth;
	int imageHeight;
	int channelNumber;

private:
	static int algoNumber;
};//class CVAlgo

NS_END

#endif//CV_ALGO_H
