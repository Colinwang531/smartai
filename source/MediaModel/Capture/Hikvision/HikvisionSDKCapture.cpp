#include "boost/make_shared.hpp"
#include "error.h"
#include "Device/Hikvision/HikvisionIPC.h"
using HikvisionIPC = NS(device, 1)::HikvisionIPC;
#include "MediaModel/Capture/Hikvision/HikvisionSDKCapture.h"

namespace framework
{
	namespace multimedia
	{
		HikvisionSDKCapture::HikvisionSDKCapture() : MediaCapture(), streamID{ 0 }
		{}

		HikvisionSDKCapture::~HikvisionSDKCapture(void)
		{}

		int HikvisionSDKCapture::openStream(
			const std::string& username, const std::string& password, 
			const std::string& ipaddr, const unsigned short port /* = 80 */, 
			const unsigned short channel /* = 0 */, void* hwnd /* = NULL */)
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

		int HikvisionSDKCapture::closeStream()
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
	}
}
