// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : ������
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

// JPEGͼƬ���ݻص�����
// �ص�����ִ�н����������ı���Ч��,�����߿ɽ��ص�����Ƶ֡�����Ȼ�����ͨ�������̴߳���.
//
// @frameData : JPEGͼƬ���ݻ���.
// @frameBytes : JPEGͼƬ�����ֽ���.
// @userData : �û�����.
// 
typedef void (CALLBACK *JPEGENCODER_PostJpegEncodeCallback)(
	const unsigned char* jpegData, const int jpegBytes, void* userData);

// ע��JPEGͼƬ�������ݻص�����
//
// @callback : �ص�����.
// @userData : �û�����.
//
// @Return
//
JPEGENCODER void JPEGENCODER_RegisterPostJpegEncodeCallback(
	JPEGENCODER_PostJpegEncodeCallback callback = NULL, void* userData = NULL);

// ����JPEGͼƬ
//
// @data : ԭʼͼƬ����,��֧��BGR24ͼƬ.
// @dataBytes : BGR24ͼƬ�ֽ���.
//
// @Return : 1��ʾ�ɹ�, �����ʾʧ��.
// 
JPEGENCODER int JPEGENCODER_EncodeJpegPicture(const unsigned char* data = NULL, const int dataBytes = 0);

#endif//JPEG_ENCODER_H
