#include "error.h"
#include "HCNetSDK.h"
#include "Device/Hikvision/HikvisionDevice.h"

NS_BEGIN(device, 1)

HikvisionDevice::HikvisionDevice(
	const char* userName /* = NULL */, const char* userPassword /* = NULL */,
	const char* deviceIP /* = NULL */, const unsigned short devicePort /* = 0 */)
	: Device(), EnableDeviceLoginAndLogout(), 
	loginUserName{ userName }, loginUserPassword{ userPassword }, loginDeviceIP{ deviceIP }, loginDevicePort{ devicePort }
{}

HikvisionDevice::~HikvisionDevice()
{}

int HikvisionDevice::createDevice()
{
	int status{ Device::createDevice() };

	//Initialize SDK when the first device was created.
	if (1 == Device::deviceNumber)
	{
		status = NET_DVR_Init() ? ERR_OK : ERR_BAD_OPERATE;
	}

	return status;
}

int HikvisionDevice::destoryDevice()
{
	int status{ Device::destoryDevice() };

	//Release SDK when the last device was destroyed.
	if (0 == Device::deviceNumber)
	{
		status = NET_DVR_Cleanup() ? ERR_OK : ERR_BAD_OPERATE;
	}

	return status;
}

int HikvisionDevice::logoutDevice()
{
	int status{ ERR_INVALID_PARAM };

	if (-1 < userID)
	{
		status = NET_DVR_Logout(userID) ? ERR_OK : ERR_BAD_OPERATE;
	}

	return status;
}

NS_END
