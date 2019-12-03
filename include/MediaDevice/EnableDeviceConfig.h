//
//		Copyright :					@2017, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@hotmail.com
//		Date :							2019-08-04
//		Description :				设备参数读写接口
//
//		History:						Author										Date													Description
//											王科威										2019-08-04									创建
//

#ifndef ENABLE_DEVICE_CONFIG_H
#define ENABLE_DEVICE_CONFIG_H

#include "predef.h"

NS_BEGIN(device, 1)

class EnableDeviceConfig
{
protected:
	EnableDeviceConfig(void) {}
	virtual ~EnableDeviceConfig(void) {}

protected:
	virtual int getNVRDigitCameras(void) = 0;
};//class EnableDeviceConfig

NS_END

#endif//ENABLE_DEVICE_CONFIG_H
