// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : ������
// E-mail : wangkw531@icloud.com
//
// SDK library for AI operating.
//

#ifndef ARITHMETIC_H
#define ARITHMETIC_H

#include <windows.h>

#ifdef ARITHMETIC_EXPORTS
#define ARITHMETIC extern "C" _declspec(dllexport)
#else
#define ARITHMETIC extern "C" _declspec(dllimport)
#endif//ARITHMETIC_EXPORTS

typedef enum class tagAlarmType_t
{
	ALARM_TYPE_NONE = 0,
	ALARM_TYPE_HELMET,
	ALARM_TYPE_PHONE,
	ALARM_TYPE_SLEEP,
	ALARM_TYPE_FIGHT,
	ALARM_TYPE_FACE
}AlarmType;

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
}AlarmInfo;

// AI�㷨��⵽�ı����������ͻص�����
// �ص�����ִ�н������������㷨ִ��Ч��,�����߿ɽ��ص��ı��������Ȼ�����ͨ�������̴߳���.
//
// @alarmInfo : ������Ϣ.
// @userData : �û�����.
// @data : BGR24ͼƬ����.
// @dataBytes : BGR24ͼƬ�ֽ���.
// 
typedef void (CALLBACK *ARITHMETIC_AlarmInfoNotifyCallback)(
	const std::vector<AlarmInfo> alarmInfo, const unsigned char* bgr24Image, const int bgr24ImageBytes,void* userData);

// ������ע���Լ���Ҫ���յ�AI�㷨����������Ϣ
//
// @alarmType : ������������.
// @callback : �������ݻص�����.
// @userData : �û�����.
//
// @Return : 1��ʾ�ɹ�,0��ʾʧ��.
// 
ARITHMETIC int ARITHMETIC_RegisterAlarmNotifyCallback(
	const AlarmType alarmType = AlarmType::ALARM_TYPE_NONE, ARITHMETIC_AlarmInfoNotifyCallback callback = NULL, void* userData = NULL);

// �������ͼƬ����,ע������ͼƬ����ǰ�����ȵ���ARITHMETIC_RegisterAlarmNotifyCallback����ע�������㷨
//
// @filePath : ����ͼƬ�洢·��.
// @faceID : �û��Զ��������ID.
//
// @Return : 1��ʾ�ɹ�,<0��ʾ������.
//
ARITHMETIC int ARITHMETIC_AddFaceImage(const char* filePath = NULL, const int faceID = 0);

// ����������ͼƬ���ݽ����㷨���
//
// @alarmType : ������������.
// @data : ��Ҫ�㷨����ͼƬ����,��֧��BGR24ͼƬ.
// @dataBytes : BGR24ͼƬ�ֽ���.
//
// @Return : 1��ʾ�ɹ�,0��ʾʧ��.
//
ARITHMETIC int ARITHMETIC_InputImageData(
	const AlarmType alarmType = AlarmType::ALARM_TYPE_NONE, const unsigned char* data = NULL, const int dataBytes = 0);

#endif//ARITHMETIC_H
