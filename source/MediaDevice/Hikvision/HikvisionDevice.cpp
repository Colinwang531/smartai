#include "HCNetSDK.h"
#include "error.h"
#include "Device/Hikvision/HikvisionDevice.h"

NS_BEGIN(device, 1)

HikvisionDevice::HikvisionDevice(
	const std::string name, const std::string password, 
	const std::string ipaddr, const unsigned short port /* = 0 */)
	: Device(), EnableDeviceLoginAndLogout(), loginUserName{ name }, loginUserPassword{ password }, 
	loginDeviceIP{ ipaddr }, loginDevicePort{ port }, loginUserID{ -1 }
{}

HikvisionDevice::~HikvisionDevice()
{}

int HikvisionDevice::createNewDevice()
{
	//Initialize SDK when the first device was created.
	if (0 == getDeviceCount())
	{
		NET_DVR_Init();
	}

	int status{ Device::createNewDevice() };

	if (ERR_OK == status)
	{
		status = loginDevice();
	}

	return status;
}

int HikvisionDevice::destoryDevice()
{
	int status{ logoutDevice() };

	if (ERR_OK == status)
	{
		status = Device::destoryDevice();
	}

	//Release SDK when the last device was destroyed.
	if (0 == getDeviceCount())
	{
		NET_DVR_Cleanup();
	}

	return status;
}

int HikvisionDevice::logoutDevice()
{
	int status{ ERR_INVALID_PARAM };

	if (-1 < loginUserID)
	{
		status = NET_DVR_Logout(loginUserID) ? ERR_OK : ERR_BAD_OPERATE;
	}

	return status;
}

NS_END
