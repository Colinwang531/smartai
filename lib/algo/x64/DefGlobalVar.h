#pragma once

#include <vector>
#include <map>

using namespace std;

typedef struct _StruInitParams
{
	int   sleepTime;          // ˯������ã�����Ϊ��λ  
	float detectThreshold;    // �����ֵ  ֵԽСԽ������Խ�������Խ��
	float trackThreshold;     // ������ֵ ֵԽС������Խ�õ����ܳ��������
	char* cfgfile;            // ���ģ�͵�cfg��ʽ�ļ���Ϊ����ṹ
	char* weightFile;         // ѵ���ļ��ģ��weights��ʽ�ļ����洢Ȩ����Ϣ
	char* savePath;           // ����������Ŀ¼
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

typedef struct _StruRect  // ���Ŀ���
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

typedef struct _StruPoint  // �������ĵ�
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
	bool bDone; // Ŀ����������
	int nCatch;  // ˯�����ר��
	vector<StruResult> vecSaveMat;
	_StruMemoryInfo()
	{
		bDone = false;
		nCatch = 0;
		vecSaveMat.clear();
	}
}StruMemoryInfo;

// ����δ����ȫñ�����Ŀ
#ifndef _FEEDBACKHELMET_H
#define _FEEDBACKHELMET_H
typedef struct _FeedBackHelmet
{
	vector<StruResult> vecShowInfo;        // ������ʾ
	map<int, StruMemoryInfo> mapMemory;    // ��ЧĿ��Ĵ洢
	_FeedBackHelmet(){
		vecShowInfo.clear();
		mapMemory.clear();
	}
}FeedBackHelmet;
#endif

// ���ڴ�绰�����Ŀ����
#ifndef _FEEDBACKPHONE_H
#define _FEEDBACKPHONE_H
typedef struct _FeedBackPhone
{
	vector<StruResult> vecShowInfo;        // ������ʾ
	map<int, StruMemoryInfo> mapMemory;    // ��ЧĿ��Ĵ洢
	_FeedBackPhone() {
		vecShowInfo.clear();
		mapMemory.clear();
	}
}FeedBackPhone;
#endif

// ���ڴ�绰�����Ŀ����
#ifndef _FEEDBACKSLEEP_H
#define _FEEDBACKSLEEP_H
typedef struct _FeedBackSleep
{
	vector<StruResult> vecShowInfo;        // ������ʾ
	map<int, StruMemoryInfo> mapMemory;    // ��ЧĿ��Ĵ洢
	_FeedBackSleep() {
		vecShowInfo.clear();
		mapMemory.clear();
	}
}FeedBackSleep;
#endif


// ���ڴ�ܼ����Ŀ����
#ifndef _FEEDBACKFIGHT_H
#define _FEEDBACKFIGHT_H
typedef struct _FeedBackFight
{
	vector<StruResult> vecShowInfo;        // ������ʾ
	map<int, StruMemoryInfo> mapMemory;    // ��ЧĿ��Ĵ洢
	_FeedBackFight() {
		vecShowInfo.clear();
		mapMemory.clear();
	}
}FeedBackFight;
#endif


//��������
#define FACE_FEATURE_LENGTH 1032

//ע������
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