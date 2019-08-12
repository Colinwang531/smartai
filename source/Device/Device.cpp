#include "Device/Device.h"

NS_BEGIN(device, 1)

int Device::counter = 0;

Device::Device()
{}

Device::~Device()
{}

int Device::createNew()
{
	return ++counter;
}

int Device::destory()
{
	return --counter;
}

NS_END
