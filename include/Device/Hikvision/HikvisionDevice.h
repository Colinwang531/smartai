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

NS_BEGIN(device, 1)

class HikvisionDevice : protected Device
{
public:
	HikvisionDevice(void);
	virtual ~HikvisionDevice(void);

public:
	virtual int login(
		const char* name = NULL, const char* password = NULL,
		const char* ip = NULL, const unsigned short port = 0,
		const bool sync = true) = 0;
	virtual int logout(void) = 0;

protected:
	//Start from 0
	int userID;
};//class HikvisionDevice

NS_END

#endif//HIKVISION_DEVICE_H
