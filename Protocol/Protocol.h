// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : ������
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

// ����ý����ID��ʾΪ�ֽ���.
//
// @url : ý����URL��ʶ.
// @callback : ý�������ݻص�����.
// @userData : �û�����.
//
// @Return : >0��ʾý����ID,=0��ʾ��������ʧ��.
// 
PROTOCOL const  PROTOCOL_Encode(const char* url = NULL);

// �ر�ý����.
//
// @streamID : ý����ID��ʶ,��ֵ��MEDIASTREAMCLIENT_OpenStream��������.
//
// @Return : 0��ʾ��������,1��ʾ�������óɹ�.
//
PROTOCOL int PROTOCOL_Decode(const char* data = NULL);

#endif//MEDIA_STREAM_CLIENT_H
