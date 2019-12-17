// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Abstract base class of media device.
//

#ifndef MEDIA_DEVICE_H
#define MEDIA_DEVICE_H

#include "boost/function.hpp"
#include "boost/shared_ptr.hpp"
#include "Mutex/RWLock.h"

namespace framework
{
	namespace multimedia
	{
		class MediaData;
		using MediaDataPtr = boost::shared_ptr<MediaData>;
		typedef boost::function<int(MediaDataPtr)> MediaDataCaptureCallback;

		class MediaDevice
		{
		public:
			MediaDevice(void);
			virtual ~MediaDevice(void);

		public:
			inline void setMediaDataCaptureCallback(MediaDataCaptureCallback callback = NULL)
			{
				mediaDataCaptureCallback = callback;
			}
			//Return : The total number of media device has been created.
			virtual int openStream(const std::string url, void* hwnd = NULL);
			virtual int closeStream(void);

		protected:
			static SharedMutex mtx;
			static int count;
			MediaDataCaptureCallback mediaDataCaptureCallback;
		};//class MediaDevice
	}//namespace multimedia
}//namespace framework

#endif//MEDIA_DEVICE_H
