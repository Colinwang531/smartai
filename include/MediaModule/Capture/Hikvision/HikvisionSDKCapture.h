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

#include "MediaModule/MediaModule.h"

namespace framework
{
	namespace multimedia
	{
		class HikvisionSDKCapture : public MediaModule
		{
		public:
			HikvisionSDKCapture(void);
			virtual ~HikvisionSDKCapture(void);

		public:
			int openStream(const std::string& streamUrl);
			int closeStream(void);
		};//class HikvisionSDKCapture
	}//namespace multimedia
}//namespace framework

#endif//HIKVISION_SDK_CAPTURE_H
