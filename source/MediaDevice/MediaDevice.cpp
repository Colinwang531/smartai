#include "error.h"
#include "Device/Device.h"

NS_BEGIN(device, 1)

SharedMutex Device::mtx;
unsigned long long Device::deviceCount = 0;

Device::Device()
{}

Device::~Device()
{}

int Device::createNewDevice()
{
	WriteLock wl{ mtx };
	++deviceCount;
	return ERR_OK;
}

int Device::destoryDevice()
{
	WriteLock wl{ mtx };
	--deviceCount;
	return ERR_OK;
}

NS_END
