// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : 王科威
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
	const unsigned char* bgr24;
	int bgr24Bytes;
}AlarmInfo;

// AI算法检测到的报警数据推送回调函数
// 回调函数执行将阻塞正常的算法执行效果,调用者可将回调的报警数据先缓存再通过其他线程处理.
//
// @alarmInfo : 报警信息.
// @userData : 用户数据.
// 
typedef void (CALLBACK *ARITHMETIC_AlarmInfoNotifyCallback)(const AlarmInfo alarmInfo, void* userData);

// 调用者注册自己需要接收的AI算法报警推送信息
//
// @alarmType : 报警数据类型.
// @callback : 报警数据回调函数.
// @userData : 用户数据.
//
// @Return : 1表示成功,0表示失败.
// 
ARITHMETIC int ARITHMETIC_RegisterAlarmNotifyCallback(
	const AlarmType alarmType = AlarmType::ALARM_TYPE_NONE, ARITHMETIC_AlarmInfoNotifyCallback callback = NULL, void* userData = NULL);

// 调用者输入图片数据进行算法检测
//
// @alarmType : 报警数据类型.
// @data : 需要算法检测的图片数据,仅支持BGR24图片.
// @dataBytes : BGR24图片字节数.
//
// @Return : 1表示成功,0表示失败.
//
ARITHMETIC int ARITHMETIC_InputImageData(
	const AlarmType alarmType = AlarmType::ALARM_TYPE_NONE, const unsigned char* data = NULL, const int dataBytes = 0);

#endif//ARITHMETIC_H
