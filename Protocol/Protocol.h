// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : 王科威
// E-mail : wangkw531@icloud.com
//
// Network protocol SDK library.
//

#ifndef PROTOCOL_H
#define PROTOCOL_H

#ifdef PROTOCOL_EXPORTS
#define PROTOCOL extern "C" _declspec(dllexport)
#else
#define PROTOCOL extern "C" _declspec(dllimport)
#endif//PROTOCOL_EXPORTS

#ifndef NULL
#define NULL 0
#endif//NULL

// 编码媒体流ID表示为字节流.
//
// @url : 媒体流URL标识.
// @callback : 媒体流数据回调函数.
// @userData : 用户数据.
//
// @Return : >0表示媒体流ID,=0表示函数调用失败.
// 
PROTOCOL const  PROTOCOL_Encode(const char* url = NULL);

// 关闭媒体流.
//
// @streamID : 媒体流ID标识,该值由MEDIASTREAMCLIENT_OpenStream函数返回.
//
// @Return : 0表示参数错误,1表示函数调用成功.
//
PROTOCOL int PROTOCOL_Decode(const char* data = NULL);

#endif//MEDIA_STREAM_CLIENT_H
