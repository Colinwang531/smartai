#include "error.h"
#include "Device/Device.h"

NS_BEGIN(device, 1)

unsigned long long Device::deviceNumber = 0;

Device::Device()
{}

Device::~Device()
{}

int Device::createDevice()
{
	mtx.lock();
	++deviceNumber;
	mtx.lock();

	return ERR_OK;
}

int Device::destoryDevice()
{
	mtx.lock();
	--deviceNumber;
	mtx.lock();

	return ERR_OK;
}

NS_END
