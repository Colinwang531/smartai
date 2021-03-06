#include "boost/make_shared.hpp"
#include "error.h"
#include "MediaDevice/Hikvision/HikvisionDevice.h"
#include "MediaModule/Capture/Hikvision/HikvisionSDKCapture.h"

namespace framework
{
	namespace multimedia
	{
		HikvisionSDKCapture::HikvisionSDKCapture() : MediaCapture()
		{}

		HikvisionSDKCapture::~HikvisionSDKCapture(void)
		{}

		int HikvisionSDKCapture::startCapture(const std::string url, void* hwnd /* = NULL */)
		{
			int status{ url.empty()? ERR_INVALID_PARAM : ERR_OK };

			if (ERR_OK == status)
			{
				MediaDevicePtr hikvisionDevicePtr{ boost::make_shared<HikvisionDevice>() };
				if (hikvisionDevicePtr)
				{
//					hikvisionDevicePtr->setMediaDataCaptureCallback(postInputMediaDataCallback);
					status = hikvisionDevicePtr->openStream(url, hwnd);
					if (ERR_OK == status)
					{
						mediaDevicePtr.swap(hikvisionDevicePtr);
					}
				}
			}

			return status;
		}

		int HikvisionSDKCapture::stopCapture()
		{
			return /*mediaDevicePtr ? mediaDevicePtr->closeStream() :*/ ERR_BAD_OPERATE;
		}
	}
}
