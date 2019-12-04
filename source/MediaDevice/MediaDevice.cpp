#include "MediaDevice/MediaDevice.h"

namespace framework
{
	namespace multimedia
	{
		SharedMutex MediaDevice::mtx;
		int MediaDevice::count = 0;

		MediaDevice::MediaDevice() : mediaDataCaptureCallback{ NULL }
		{}

		MediaDevice::~MediaDevice()
		{}

		int MediaDevice::openStream(const std::string& streamUrl)
		{
			WriteLock wl{ mtx };
			return ++count;
		}

		int MediaDevice::closeStream()
		{
			WriteLock wl{ mtx };
			return 0 < count ? --count : count;
		}
	}//namespace multimedia
}//namespace framework
