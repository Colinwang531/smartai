// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : ������
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

// ý�������ݻص�����.
//
// @streamID : ý����ID.
// @event : TCP�¼�����.
// @data : ֡���ݻ���.
// @dataBytes : ֡�����ֽ���.
// @userData : �û�����.
// 
typedef void (CALLBACK *MEDIASTREAMCLIENT_MediaStreamDataCallback)(
	const int streamID, const unsigned char event, const unsigned char* data, 
	const unsigned long long dataBytes, void* userData);

// ��ý����.
//
// @url : ý����URL��ʶ.
// @callback : ý�������ݻص�����.
// @userData : �û�����.
//
// @Return : >0��ʾý����ID,=0��ʾ��������ʧ��.
// 
MEDIASTREAMCLIENT int MEDIASTREAMCLIENT_OpenStream(
	const char* url = NULL, MEDIASTREAMCLIENT_MediaStreamDataCallback callback = NULL, void* userData = NULL);

// �ر�ý����.
//
// @streamID : ý����ID��ʶ,��ֵ��MEDIASTREAMCLIENT_OpenStream��������.
//
// @Return : 0��ʾ��������,1��ʾ�������óɹ�.
//
MEDIASTREAMCLIENT int MEDIASTREAMCLIENT_CloseStream(const int streamID = 0);

#endif//MEDIA_STREAM_CLIENT_H
