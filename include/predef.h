//
//		Copyright :					@2018, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2017-11-29
//		Description:
//
//		History:						Author										Date													Description
//											王科威										2017-11-29									创建
//

#ifndef PREDEF_H
#define PREDEF_H

#define NS_BEGIN(name, version) namespace NS_##name##_##version{
#define NS_END }
#define NS(name, version) NS_##name##_##version

/***
// 整数
using Int8 = signed char;
using Int16 = signed short;
using int = signed int;
using Long32 = signed long;
using Int64 = signed long long;
using UInt8 = unsigned char;
using unsigned short = unsigned short;
using unsigned int = unsigned int;
using UInt64 = unsigned long long;
//字符串
using const char* = const char*;
using PChar = char*;

#ifndef _WINDOWS
using HANDLE = void*;
using SOCKET = void*;
using NULL = 0;
#else
//#include <windows.h>
#ifndef NULL
#define NULL 0
#endif//NULL
#endif//_WINDOWS

static const unsigned int maxTransportMTUBytes = 65530;
static const unsigned short gMaxBytesOfMediaHeader = 12;
static const unsigned int gMaxBytesOfMediaData = 3 * 1024 * 1024;
static const unsigned int gMaxURIBytes = 1024;
static const unsigned int gMaxFilePath = 260;
***/

#ifndef NULL
#define NULL 0
#endif//NULL

static const unsigned short minPortNumber = 5000;
static const unsigned short maxPortNumber = 65535;
static const unsigned int maxTransportMTUBytes = 65530;

typedef struct tagBGR24Frame_t
{
	char* frameData;
	int frameBytes;
	char* jpegData;
	int jpegBytes;
	int channelIndex;
	char* NVRIp;
}BGR24Frame;

#endif//PREDEF_H
