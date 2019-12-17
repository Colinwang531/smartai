// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : ������
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

// ��Ƶ֡���ݻص�����
// �ص�����ִ�н����������Ĳ���Ч��,�����߿ɽ��ص�����Ƶ֡�����Ȼ�����ͨ�������̴߳���.
//
// @playID : ����ID.
// @frameType : ֡��������,0��ʾ��Ƶ,1��ʾ��Ƶ.
// @frameData : ֡���ݻ���.
// @frameBytes : ֡�����ֽ���.
// @userData : �û�����.
// 
typedef void (CALLBACK *MEDIAPLAY_MediaFrameCallback)(
	const int playID, const unsigned char frameType, const unsigned char* frameData, const int frameBytes, void* userData);

// ��ý����.
//
// @url : ý����URL��ʶ.
// @hwnd : ��Ƶ���ŵĴ��ھ��,�ò�����������,����ú���������ʧ��.
// @callback : BGRͼƬ���ݻص�����.
// @userData : �û�����.
//
// @Return : >0��ʾ����ID,=0��ʾ��������ʧ��.
// 
MEDIAPLAY int MEDIAPLAY_OpenStream(
	const char* url = NULL, const HWND hwnd = NULL, MEDIAPLAY_MediaFrameCallback callback = NULL, void* userData = NULL);

// �ر�ý��ʵʱ������
//
// @playID : ����ID,��ֵ��MEDIAPLAY_OpenStream��������ֵ����.
//
// @Return : 0��ʾ��������,1��ʾ�������óɹ�.
//
MEDIAPLAY int MEDIAPLAY_CloseStream(const int playID = 0);

#endif//MEDIA_PLAY_H
