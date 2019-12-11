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

#include "MediaModule/Capture/MediaCapture.h"

namespace framework
{
	namespace multimedia
	{
		class HikvisionSDKCapture : public MediaCapture
		{
		public:
			HikvisionSDKCapture(void);
			~HikvisionSDKCapture(void);

		public:
			int startCapture(const std::string& streamUrl) override;
			int stopCapture(void) override;
		};//class HikvisionSDKCapture
	}//namespace multimedia
}//namespace framework

#endif//HIKVISION_SDK_CAPTURE_H
