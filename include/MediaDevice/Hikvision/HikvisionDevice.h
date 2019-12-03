// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Base class of HIKVISION device.
//

#ifndef HIKVISION_DEVICE_H
#define HIKVISION_DEVICE_H

#include "Device/Device.h"
#include "Device/EnableDeviceLoginAndLogout.h"

NS_BEGIN(device, 1)

class HikvisionDevice 
	: public Device, protected EnableDeviceLoginAndLogout
{
public:
	HikvisionDevice(
		const std::string name, const std::string password,
		const std::string ipaddr, const unsigned short port = 0);
	virtual ~HikvisionDevice(void);

	inline int getUserID(void) const 
	{
		return loginUserID;
	}

protected:
	int createNewDevice(void) override;
	int destoryDevice(void) override;
	int logoutDevice(void) override;

protected:
	const std::string loginUserName;
	const std::string loginUserPassword;
	const std::string loginDeviceIP;
	const unsigned short loginDevicePort;
	int loginUserID;
};//class HikvisionDevice

NS_END

#endif//HIKVISION_DEVICE_H
