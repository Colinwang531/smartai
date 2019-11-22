#include "boost/make_shared.hpp"
#include "error.h"
#include "Device/Hikvision/HikvisionIPC.h"
using HikvisionIPC = NS(device, 1)::HikvisionIPC;
#include "MediaModel/Accessor/SDK/HikvisionIPCMediaAccessor.h"

NS_BEGIN(model, 1)

HikvisionIPCMediaAccessor::HikvisionIPCMediaAccessor() : MediaAccessor(), streamID{ 0 }
{}

HikvisionIPCMediaAccessor::~HikvisionIPCMediaAccessor(void)
{
	closeStream();
}

int HikvisionIPCMediaAccessor::openStream(
	const std::string name, const std::string password, 
	const std::string address, const int port /* = 8000 */, 
	const int channel /* = 0 */, void* hwnd /* = NULL */)
{
	int status{ MediaAccessor::openStream(name, password, address, port, channel) };

	if (ERR_OK == status)
	{
		DevicePtr hikvisionIPCDevicePtr{
			boost::make_shared<HikvisionIPC>(name, password, address, port) };
		if (hikvisionIPCDevicePtr)
		{
			status = hikvisionIPCDevicePtr->createNewDevice();
			if (ERR_OK == status)
			{
				boost::shared_ptr<NS(device, 1)::HikvisionIPC> hikvisionIPCPtr{ 
					boost::dynamic_pointer_cast<NS(device, 1)::HikvisionIPC>(hikvisionIPCDevicePtr) };
				if (ERR_OK == hikvisionIPCPtr->openStream(hwnd))
				{
					hikvisionIPCPtr->setPostInputMediaDataCallback(postInputMediaDataCallback);
					devicePtr.swap(hikvisionIPCDevicePtr);
				}
				else
				{
					status = ERR_BAD_OPERATE;
				}
			}
		}
	}

	return status;
}

int HikvisionIPCMediaAccessor::closeStream()
{
	if (devicePtr)
	{
		boost::shared_ptr<NS(device, 1)::HikvisionIPC> hikvisionIPCPtr{
					boost::dynamic_pointer_cast<NS(device, 1)::HikvisionIPC>(devicePtr) };
		hikvisionIPCPtr->closeStream();
		devicePtr->destoryDevice();
	}

	return MediaAccessor::closeStream();
}

int HikvisionIPCMediaAccessor::inputMediaData(MediaDataPtr mediaData)
{
	return ERR_NOT_SUPPORT;
}

NS_END
