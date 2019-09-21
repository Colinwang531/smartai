#pragma once

#include <vector>
#include <map>

using namespace std;

typedef struct _StruInitParams
{
	int   sleepTime;          // 睡觉检测用，以秒为单位  
	float detectThreshold;    // 检测阈值  值越小越灵敏，越大误捡率越少
	float trackThreshold;     // 跟踪阈值 值越小，跟踪越久但可能出现误跟踪
	char* cfgfile;            // 检测模型的cfg格式文件，为网络结构
	char* weightFile;         // 训练的检测模型weights格式文件，存储权重信息
	char* savePath;           // 处理结果保存目录
	_StruInitParams()
	{
		sleepTime = 10;
		detectThreshold = 0.2f;
		trackThreshold = 0.3f;
		cfgfile = nullptr;
		weightFile = nullptr;
		savePath = NULL;
	}
}StruInitParams;

typedef struct _StruRect  // 检测目标框
{
	int x;
	int y;
	int width;
	int height;
	_StruRect()
	{
		x = y = width = height = 0;
	}
	_StruRect(int x, int y, int w, int h)
	{
		x = x;
		y = y;
		width = w;
		height = h;
	}
}StruRect;

typedef struct _StruPoint  // 检测框中心点
{
	int x;
	int y;
	_StruPoint()
	{
		x = y = 0;
	}
}StruPoint;


typedef struct _StruResult
{
	int nLabel;      
	int nSerioNo;    
	float detectConfidence;
	float trackConfidence;
	time_t catchTime;
	StruRect rRect;
	vector<StruPoint> vecTracePoint;  
	char *pUcharImage;
	_StruResult()
	{
		nLabel = 0;
		nSerioNo = 0;
		detectConfidence = 0.f;
		trackConfidence = 0.f;
		vecTracePoint.clear();
		pUcharImage = nullptr;
	}
}StruResult;

typedef struct _StruMemoryInfo
{
	bool bDone; // 目标检测跟踪完成
	int nCatch;  // 睡觉检测专用
	vector<StruResult> vecSaveMat;
	_StruMemoryInfo()
	{
		bDone = false;
		nCatch = 0;
		vecSaveMat.clear();
	}
}StruMemoryInfo;

// 用于未戴安全帽检测项目
#ifndef _FEEDBACKHELMET_H
#define _FEEDBACKHELMET_H
typedef struct _FeedBackHelmet
{
	vector<StruResult> vecShowInfo;        // 用于显示
	map<int, StruMemoryInfo> mapMemory;    // 有效目标的存储
	_FeedBackHelmet(){
		vecShowInfo.clear();
		mapMemory.clear();
	}
}FeedBackHelmet;
#endif

// 用于打电话检测项目参数
#ifndef _FEEDBACKPHONE_H
#define _FEEDBACKPHONE_H
typedef struct _FeedBackPhone
{
	vector<StruResult> vecShowInfo;        // 用于显示
	map<int, StruMemoryInfo> mapMemory;    // 有效目标的存储
	_FeedBackPhone() {
		vecShowInfo.clear();
		mapMemory.clear();
	}
}FeedBackPhone;
#endif

// 用于打电话检测项目参数
#ifndef _FEEDBACKSLEEP_H
#define _FEEDBACKSLEEP_H
typedef struct _FeedBackSleep
{
	vector<StruResult> vecShowInfo;        // 用于显示
	map<int, StruMemoryInfo> mapMemory;    // 有效目标的存储
	_FeedBackSleep() {
		vecShowInfo.clear();
		mapMemory.clear();
	}
}FeedBackSleep;
#endif


// 用于打架检测项目参数
#ifndef _FEEDBACKFIGHT_H
#define _FEEDBACKFIGHT_H
typedef struct _FeedBackFight
{
	vector<StruResult> vecShowInfo;        // 用于显示
	map<int, StruMemoryInfo> mapMemory;    // 有效目标的存储
	_FeedBackFight() {
		vecShowInfo.clear();
		mapMemory.clear();
	}
}FeedBackFight;
#endif


//特征长度
#define FACE_FEATURE_LENGTH 1032

//注册人脸
typedef struct _StruFaceInfo
{
	int faceId;
	StruRect faceRect;
	unsigned char faceFeature[FACE_FEATURE_LENGTH];
	_StruFaceInfo()
	{
		memset(faceFeature, 0, FACE_FEATURE_LENGTH);
	}
}StruFaceInfo;