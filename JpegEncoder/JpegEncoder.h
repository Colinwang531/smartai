// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : 王科威
// E-mail : wangkw531@icloud.com
//
// SDK library for encoding JPEG picture.
//

#ifndef JPEG_ENCODER_H
#define JPEG_ENCODER_H

#include <windows.h>

#ifdef JPEG_ENCODER_EXPORTS
#define JPEGENCODER extern "C" _declspec(dllexport)
#else
#define JPEGENCODER extern "C" _declspec(dllimport)
#endif//JPEG_ENCODER_EXPORTS

// JPEG图片数据回调函数
// 回调函数执行将阻塞正常的编码效果,调用者可将回调的视频帧数据先缓存再通过其他线程处理.
//
// @frameData : JPEG图片数据缓存.
// @frameBytes : JPEG图片数据字节数.
// @userData : 用户数据.
// 
typedef void (CALLBACK *JPEGENCODER_PostJpegEncodeCallback)(
	const unsigned char* jpegData, const int jpegBytes, void* userData);

// 注册JPEG图片编码数据回调函数
//
// @callback : 回调函数.
// @userData : 用户数据.
//
// @Return
//
JPEGENCODER void JPEGENCODER_RegisterPostJpegEncodeCallback(
	JPEGENCODER_PostJpegEncodeCallback callback = NULL, void* userData = NULL);

// 编码JPEG图片
//
// @data : 原始图片数据,仅支持BGR24图片.
// @dataBytes : BGR24图片字节数.
//
// @Return : 1表示成功, 否则表示失败.
// 
JPEGENCODER int JPEGENCODER_EncodeJpegPicture(const unsigned char* data = NULL, const int dataBytes = 0);

#endif//JPEG_ENCODER_H
