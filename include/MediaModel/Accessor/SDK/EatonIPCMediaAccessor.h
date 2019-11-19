// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// AV live stream accessor using EATON SDK.
//

#ifndef EATON_IPC_MEDIA_ACCESSOR_H
#define EATON_IPC_MEDIA_ACCESSOR_H

#include "MediaModel/Accessor/MediaAccessor.h"

NS_BEGIN(model, 1)

class EatonIPCMediaAccessor : public MediaAccessor
{
public:
	EatonIPCMediaAccessor(void);
	virtual ~EatonIPCMediaAccessor(void);

public:
	int openStream(
		const std::string name, const std::string password,
		const std::string address, const int port = 8000,
		const int channel = 0) override;
	int closeStream(void) override;

protected:
	int inputMediaData(MediaDataPtr mediaData) override;

private:
	int initializeEatonSDK(const std::string filePath);
	int loginIPCDevice(
		const std::string name, const std::string password,
		const std::string address, const int port = 8000);
	static void CALLBACK DetectConnect(
		unsigned long dwIP, unsigned short wPort, unsigned long dwHandle, 
		unsigned long dwCBconnectType, unsigned long dwDataLen, unsigned long dwData, void* pContext);

private:
	unsigned long streamID;
};//class EatonIPCMediaAccessor

NS_END

#endif//EATON_IPC_MEDIA_ACCESSOR_H
