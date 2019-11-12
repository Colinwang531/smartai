// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : 王科威
// E-mail : wangkw531@icloud.com
//
// SDK library for playing video and audio stream.
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
// @frameData : YUV420P视频帧数据缓存.
// @frameBytes : 视频帧数据字节数.
// 
typedef void (CALLBACK *MEDIAPLAY_VideoFrameCallback)(const char* frameData, const int frameBytes);

// 打开媒体文件播放
//
// @url : 本地媒体文件的完整路径.
// @hwnd : 视频播放的窗口句柄,该参数必须设置,否则该函数将调用失败.
// @callback : YUV420P视频帧数据回调函数.
//
// @Return : >0表示播放ID,=0表示函数调用失败.
// 
MEDIAPLAY int MEDIAPLAY_StartPlay(
	const char* url = NULL, const HWND hwnd = NULL, MEDIAPLAY_VideoFrameCallback callback = NULL);

// 关闭媒体文件播放
//
// @playID : 播放ID,该值由MEDIAPLAY_StartPlay函数返回值生产.
//
// @Return : 0表示参数错误,1表示函数调用成功.
//
MEDIAPLAY int MEDIAPLAY_StopPlay(const int playID = 0);

#endif//MEDIA_PLAY_H
