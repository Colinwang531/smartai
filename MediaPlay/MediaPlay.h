// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : 王科威
// E-mail : wangkw531@icloud.com
//
// Media SDK library for playing video and audio stream.
//

#ifndef MEDIA_PLAY_H
#define MEDIA_PLAY_H

#include <windows.h>

#ifdef MEDIA_PLAY_EXPORTS
#define MEDIAPLAY extern "C" _declspec(dllexport)
#else
#define MEDIAPLAY extern "C" _declspec(dllimport)
#endif//MEDIA_PLAY_EXPORTS

// 视频帧数据回调函数
// 回调函数执行将阻塞正常的播放效果,调用者可将回调的视频帧数据先缓存再通过其他线程处理.
//
// @playID : 播放ID.
// @frameType : 帧数据类型,0表示视频,1表示音频.
// @frameData : 帧数据缓存.
// @frameBytes : 帧数据字节数.
// @userData : 用户数据.
// 
typedef void (CALLBACK *MEDIAPLAY_MediaFrameCallback)(
	const int playID, const unsigned char frameType, const unsigned char* frameData, const int frameBytes, void* userData);

// 打开媒体流.
//
// @url : 媒体流URL标识.
// @hwnd : 视频播放的窗口句柄,该参数必须设置,否则该函数将调用失败.
// @callback : BGR图片数据回调函数.
// @userData : 用户数据.
//
// @Return : >0表示播放ID,=0表示函数调用失败.
// 
MEDIAPLAY int MEDIAPLAY_OpenStream(
	const char* url = NULL, const HWND hwnd = NULL, MEDIAPLAY_MediaFrameCallback callback = NULL, void* userData = NULL);

// 关闭媒体实时流播放
//
// @playID : 播放ID,该值由MEDIAPLAY_OpenStream函数返回值生产.
//
// @Return : 0表示参数错误,1表示函数调用成功.
//
MEDIAPLAY int MEDIAPLAY_CloseStream(const int playID = 0);

#endif//MEDIA_PLAY_H
