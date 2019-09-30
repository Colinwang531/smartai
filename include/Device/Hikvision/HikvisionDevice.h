//
//		Copyright :					@2018, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2017-06-26
//		Description :				海康设备类
//
//		History:						Author										Date													Description
//											王科威										2017-06-26									创建
//

#ifndef HIKVISION_DEVICE_H
#define HIKVISION_DEVICE_H

#include "Device/Device.h"
#include "Device/EnableDeviceLoginAndLogout.h"

NS_BEGIN(device, 1)

class HikvisionDevice : public Device, protected EnableDeviceLoginAndLogout
{
public:
	HikvisionDevice(
		const char* userName = NULL, const char* userPassword = NULL,
		const char* deviceIP = NULL, const unsigned short devicePort = 0);
	virtual ~HikvisionDevice(void);

	inline int getUserID(void) const 
	{
		return userID;
	}

protected:
	int createDevice(void) override;
	int destoryDevice(void) override;
	int logoutDevice(void) override;

protected:
	const std::string loginUserName;
	const std::string loginUserPassword;
	const std::string loginDeviceIP;
	const unsigned short loginDevicePort;
	int userID;
};//class HikvisionDevice

NS_END

#endif//HIKVISION_DEVICE_H
