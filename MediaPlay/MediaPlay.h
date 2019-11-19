// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : ������
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

// ��Ƶ֡���ݻص�����
// �ص�����ִ�н����������Ĳ���Ч��,�����߿ɽ��ص�����Ƶ֡�����Ȼ�����ͨ�������̴߳���.
//
// @frameData : YUV420P��Ƶ֡���ݻ���.
// @frameBytes : ��Ƶ֡�����ֽ���.
// 
typedef void (CALLBACK *MEDIAPLAY_VideoFrameCallback)(const char* frameData, const int frameBytes);

// ��ý���ļ�����
//
// @url : ����ý���ļ�������·��.
// @hwnd : ��Ƶ���ŵĴ��ھ��,�ò�����������,����ú���������ʧ��.
// @callback : YUV420P��Ƶ֡���ݻص�����.
//
// @Return : >0��ʾ����ID,=0��ʾ��������ʧ��.
// 
MEDIAPLAY int MEDIAPLAY_StartPlay(
	const char* url = NULL, const HWND hwnd = NULL, MEDIAPLAY_VideoFrameCallback callback = NULL);

// �ر�ý���ļ�����
//
// @playID : ����ID,��ֵ��MEDIAPLAY_StartPlay��������ֵ����.
//
// @Return : 0��ʾ��������,1��ʾ�������óɹ�.
//
MEDIAPLAY int MEDIAPLAY_StopPlay(const int playID = 0);

// ��ý��ʵʱ������
//
// @name : ʵʱ���豸��¼�û���.
// @password : ʵʱ���豸��¼�û�����.
// @address : ʵʱ���豸IP��ַ.
// @port : ʵʱ���豸�˿ں�.
// @channel : ʵʱ��������,��0��ʼ���豸��֧�ֵ����ͨ������.
// @hwnd : ��Ƶ���ŵĴ��ھ��,�ò�����������,����ú���������ʧ��.
// @callback : YUV420P��Ƶ֡���ݻص�����.
//
// @Return : >0��ʾ����ID,=0��ʾ��������ʧ��.
// 
MEDIAPLAY int MEDIAPLAY_StartLivestreamPlay(
	const char* name = NULL, const char* password = NULL, 
	const char* address = NULL, const int port = 8000,
	const int channel = 0, const HWND hwnd = NULL, 
	MEDIAPLAY_VideoFrameCallback callback = NULL);

// �ر�ý��ʵʱ������
//
// @playID : ����ID,��ֵ��MEDIAPLAY_StartLivestreamPlay��������ֵ����.
//
// @Return : 0��ʾ��������,1��ʾ�������óɹ�.
//
MEDIAPLAY int MEDIAPLAY_StopLivestreamPlay(const int playID = 0);

#endif//MEDIA_PLAY_H
