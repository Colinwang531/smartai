//
//		Copyright :					@2017, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@hotmail.com
//		Date :							2019-08-04
//		Description :				�豸������д�ӿ�
//
//		History:						Author										Date													Description
//											������										2019-08-04									����
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
