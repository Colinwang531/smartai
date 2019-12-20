// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : 王科威
// E-mail : wangkw531@icloud.com
//
// Media strem client SDK library for playing video and audio stream.
//

#ifndef MEDIA_STREAM_CLIENT_H
#define MEDIA_STREAM_CLIENT_H

#ifdef MEDIA_STREAM_CLIENT_EXPORTS
#define MEDIASTREAMCLIENT extern "C" _declspec(dllexport)
#else
#define MEDIASTREAMCLIENT extern "C" _declspec(dllimport)
#endif//MEDIA_PLAY_EXPORTS

#ifndef NULL
#define NULL 0
#endif//NULL

#ifndef CALLBACK
#define CALLBACK __stdcall
#endif//CALLBACK

typedef enum class tagTCPClientEvent_t : unsigned char
{
	TCP_CLIENT_EVENT_NONE = 0,
	TCP_CLIENT_EVENT_CONNECTED,
	TCP_CLIENT_EVENT_DISCONNECTED,
	TCP_CLIENT_EVENT_READ
}TCPClientEvent;

// 媒体流数据回调函数.
//
// @streamID : 媒体流ID.
// @event : TCP事件类型.
// @data : 帧数据缓存.
// @dataBytes : 帧数据字节数.
// @userData : 用户数据.
// 
typedef void (CALLBACK *MEDIASTREAMCLIENT_MediaStreamDataCallback)(
	const int streamID, const unsigned char event, const unsigned char* data, 
	const unsigned long long dataBytes, void* userData);

// 打开媒体流.
//
// @url : 媒体流URL标识.
// @callback : 媒体流数据回调函数.
// @userData : 用户数据.
//
// @Return : >0表示媒体流ID,=0表示函数调用失败.
// 
MEDIASTREAMCLIENT int MEDIASTREAMCLIENT_OpenStream(
	const char* url = NULL, MEDIASTREAMCLIENT_MediaStreamDataCallback callback = NULL, void* userData = NULL);

// 关闭媒体流.
//
// @streamID : 媒体流ID标识,该值由MEDIASTREAMCLIENT_OpenStream函数返回.
//
// @Return : 0表示参数错误,1表示函数调用成功.
//
MEDIASTREAMCLIENT int MEDIASTREAMCLIENT_CloseStream(const int streamID = 0);

#endif//MEDIA_STREAM_CLIENT_H
