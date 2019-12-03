// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// AV live stream capture using HIKVISION SDK.
//

#ifndef HIKVISION_SDK_CAPTURE_H
#define HIKVISION_SDK_CAPTURE_H

#include "boost/shared_ptr.hpp"
#include "Device/Device.h"
using DevicePtr = boost::shared_ptr<NS(device, 1)::Device>;
#include "MediaModel/Capture/MediaCapture.h"

namespace framework
{
	namespace multimedia
	{
		class HikvisionSDKCapture : public MediaCapture
		{
		public:
			HikvisionSDKCapture(void);
			virtual ~HikvisionSDKCapture(void);

		protected:
			int openStream(
				const std::string& username, const std::string& password,
				const std::string& ipaddr, const unsigned short port = 80, 
				const unsigned short channel = 0, void* hwnd = NULL) override;
			int closeStream(void) override;

		private:
			DevicePtr devicePtr;
			unsigned long streamID;
		};//class HikvisionSDKCapture
	}//namespace multimedia
}//namespace framework

#endif//HIKVISION_SDK_CAPTURE_H
