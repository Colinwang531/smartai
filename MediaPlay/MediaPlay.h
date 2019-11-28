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
// @playID : ����ID.
// @frameData : BGR24ͼƬ���ݻ���.
// @frameBytes : BGR24ͼƬ�����ֽ���.
// @userData : �û�����.
// 
typedef void (CALLBACK *MEDIAPLAY_VideoFrameCallback)(
	const int playID, const unsigned char* frameData, const int frameBytes, void* userData);

// ��ý���ļ�����
//
// @filePath : ����ý���ļ�������·��.
// @hwnd : ��Ƶ���ŵĴ��ھ��,�ò�����������,����ú���������ʧ��.
// @callback : YUV420P��Ƶ֡���ݻص�����.
//
// @Return : >0��ʾ����ID,=0��ʾ��������ʧ��.
// 
MEDIAPLAY int MEDIAPLAY_StartPlayback(
	const char* filePath = NULL, const HWND hwnd = NULL, MEDIAPLAY_VideoFrameCallback callback = NULL);

// �ر�ý���ļ�����
//
// @playID : ����ID,��ֵ��MEDIAPLAY_StartPlay��������ֵ����.
//
// @Return : 0��ʾ��������,1��ʾ�������óɹ�.
//
MEDIAPLAY int MEDIAPLAY_StopPlayback(const int playID = 0);

// ��ý��ʵʱ������
//
// @name : ʵʱ���豸��¼�û���.
// @password : ʵʱ���豸��¼�û�����.
// @address : ʵʱ���豸IP��ַ.
// @port : ʵʱ���豸�˿ں�.
// @channel : ʵʱ��������,��1��ʼ���豸��֧�ֵ����ͨ������.
// @hwnd : ��Ƶ���ŵĴ��ھ��,�ò�����������,����ú���������ʧ��.
// @callback : BGRͼƬ���ݻص�����.
// @userData : �û�����.
//
// @Return : >0��ʾ����ID,=0��ʾ��������ʧ��.
// 
MEDIAPLAY int MEDIAPLAY_StartLivestream(
	const char* name = NULL, const char* password = NULL, 
	const char* address = NULL, const int port = 8000,
	const int channel = 0, const HWND hwnd = NULL, 
	MEDIAPLAY_VideoFrameCallback callback = NULL,
	void* userData = NULL);

// �ر�ý��ʵʱ������
//
// @playID : ����ID,��ֵ��MEDIAPLAY_StartLivestreamPlay��������ֵ����.
//
// @Return : 0��ʾ��������,1��ʾ�������óɹ�.
//
MEDIAPLAY int MEDIAPLAY_StopLivestream(const int playID = 0);

#endif//MEDIA_PLAY_H
