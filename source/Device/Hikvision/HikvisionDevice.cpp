#include "HCNetSDK.h"
#include "Device/Hikvision/HikvisionDevice.h"

NS_BEGIN(device, 1)

HikvisionDevice::HikvisionDevice() : Device(), userID{ -1 }
{
	//Initialize SDK of device when first object was created.
	if (1 == Device::createNew())
	{
		NET_DVR_Init();
	}
}

HikvisionDevice::~HikvisionDevice()
{
	//Release resources when the last object was destroyed.
	if (0 == Device::destory())
	{
		NET_DVR_Cleanup();
	}
}

NS_END
