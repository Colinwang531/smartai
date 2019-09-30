//
//		Copyright :					@2018, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2017-06-26
//		Description :				设备抽象基类，所有物理硬件设备都可以直接继承于此
//
//		History:						Author										Date													Description
//											王科威										2017-06-26									创建
//

#ifndef DEVICE_H
#define DEVICE_H

#include "boost/thread/mutex.hpp"
#include "predef.h"

NS_BEGIN(device, 1)

class Device
{
public:
	Device(void);
	virtual ~Device(void);

public:
	virtual int createDevice(void);
	virtual int destoryDevice(void);

protected:
	boost::mutex mtx;
	static unsigned long long deviceNumber;
};//class Device

NS_END

#endif//DEVICE_H
