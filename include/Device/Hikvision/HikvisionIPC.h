// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Base class of HIKVISION IP camera device.
//

#ifndef HIKVISION_IPC_H
#define HIKVISION_IPC_H

#include "Device/Hikvision/HikvisionDevice.h"

NS_BEGIN(device, 1)

class HikvisionIPC : public HikvisionDevice
{
public:
	HikvisionIPC(
		const std::string name, const std::string password,
		const std::string ipaddr, const unsigned short port = 0);
	virtual ~HikvisionIPC(void);

public:
	int openStream(void* hwnd = NULL);
	int closeStream(void);

protected:
	int loginDevice(void) override;

private:
	static void CALLBACK realplayDataCallback(
		long streamID, unsigned long dataType, unsigned char* data, unsigned long dataBytes, void* pUser);

private:
	long streamID;
};//class HikvisionIPC

NS_END

#endif//HIKVISION_IPC_H
