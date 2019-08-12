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

#include "predef.h"

NS_BEGIN(device, 1)

class Device
{
public:
	Device(void);
	virtual ~Device(void);

protected:
	int createNew(void);
	int destory(void);

protected:
	static int counter;
};//class Device

NS_END

#endif//DEVICE_H
