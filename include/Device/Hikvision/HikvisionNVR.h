//
//		Copyright :					@2018, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2019-07-19
//		Description :				海康NVR类
//
//		History:						Author										Date													Description
//											王科威										2019-07-19									创建
//

#ifndef HIKVISION_NVR_H
#define HIKVISION_NVR_H

#include <vector>
#include "Device/EnableDeviceConfig.h"
#include "Device/Hikvision/HikvisionDevice.h"

NS_BEGIN(device, 1)

class Camera;

class HikvisionNVR : public HikvisionDevice, protected EnableDeviceConfig
{
public:
	HikvisionNVR(
		const char* userName = NULL, const char* userPassword = NULL,
		const char* deviceIP = NULL, const unsigned short devicePort = 0);
	virtual ~HikvisionNVR(void);

public:
	inline const std::vector<Camera> getDigitCameras(void) const 
	{
		return digitCameras;
	}

protected:
	int loginDevice(void) override;
	int getNVRDigitCameras(void) override;

private:
	std::vector<Camera> digitCameras;
};//class HikvisionNVR

NS_END

#endif//HIKVISION_NVR_H
