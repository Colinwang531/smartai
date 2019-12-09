#include "boost/make_shared.hpp"
#include "error.h"
#include "MediaDevice/Hikvision/HikvisionDevice.h"
#include "MediaModel/Capture/Hikvision/HikvisionSDKCapture.h"

namespace framework
{
	namespace multimedia
	{
		HikvisionSDKCapture::HikvisionSDKCapture() : MediaCapture()
		{}

		HikvisionSDKCapture::~HikvisionSDKCapture(void)
		{}

		int HikvisionSDKCapture::openStream(const std::string& streamUrl)
		{
			int status{ ERR_OK };

			if (ERR_OK == status)
			{
				MediaDevicePtr hikvisionDevicePtr{ boost::make_shared<HikvisionDevice>() };
				if (hikvisionDevicePtr)
				{
					hikvisionDevicePtr->setMediaDataCaptureCallback(postInputMediaDataCallback);
					status = hikvisionDevicePtr->openStream(streamUrl);
					if (ERR_OK == status)
					{
						mediaDevicePtr.swap(hikvisionDevicePtr);
					}
				}
			}

			return status;
		}

		int HikvisionSDKCapture::closeStream()
		{
			return mediaDevicePtr ? mediaDevicePtr->closeStream() : ERR_BAD_OPERATE;
		}
	}
}
