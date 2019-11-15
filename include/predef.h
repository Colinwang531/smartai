// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Global definitions.
//

#ifndef PREDEF_H
#define PREDEF_H

#define NS_BEGIN(name, version) namespace NS_##name##_##version{
#define NS_END }
#define NS(name, version) NS_##name##_##version

#ifdef _WINDOWS
#ifndef CALLBACK
#define CALLBACK __stdcall
#endif//CALLBACK
#ifndef NULL
#define NULL 0
#endif//NULL
#else
#define CALLBACK
#endif//_WINDOWS

static const unsigned short minPortNumber = 5000;
static const unsigned short maxPortNumber = 65535;
// WARNING : REQ <-> REP mode is not compatible for splitting package of data.
static const unsigned int maxTransportMTUBytes = 1024 * 1024/*65530*/;

typedef struct tagBGR24Frame_t
{
	char* frameData;
	int frameBytes;
	char* jpegData;
	int jpegBytes;
	int channelIndex;
	char* NVRIp;
}BGR24Frame;

typedef enum class tagMediaDataMainID_t
{
	MEDIA_DATA_MAIN_ID_NONE = 0,
	MEDIA_DATA_MAIN_ID_VIDEO,
	MEDIA_DATA_MAIN_ID_AUDIO,
	MEDIA_DATA_MAIN_ID_IMAGE
}MediaDataMainID;

typedef enum class tagMediaDataSubID_t
{
	MEDIA_DATA_SUB_ID_NONE = 0,
	MEDIA_DATA_SUB_ID_H264,
	MEDIA_DATA_SUB_ID_H265,
	MEDIA_DATA_SUB_ID_YV12,
	MEDIA_DATA_SUB_ID_YUV420P,
	MEDIA_DATA_SUB_ID_AAC,
	MEDIA_DATA_SUB_ID_G722,
	MEDIA_DATA_SUB_ID_JPEG,
	MEDIA_DATA_SUB_ID_BMP
}MediaDataSubID;

// Patch ID specifies the type of video frame in detail.
typedef enum class tagMediaDataPatchID_t
{
	MEDIA_DATA_PATCH_ID_NONE = 0,
	MEDIA_DATA_PATCH_ID_SPS,
	MEDIA_DATA_PATCH_ID_PPS,
	MEDIA_DATA_PATCH_ID_I,
	MEDIA_DATA_PATCH_ID_B,
	MEDIA_DATA_PATCH_ID_P,
}MediaDataPatchID;

#endif//PREDEF_H
