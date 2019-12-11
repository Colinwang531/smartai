// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Abstract base class of data capturing module.
//

#ifndef MEDIA_CAPTURE_H
#define MEDIA_CAPTURE_H

#include "MediaModule/MediaModule.h"

namespace framework
{
	namespace multimedia
	{
		class MediaCapture : public MediaModule
		{
		public:
			MediaCapture(void) : MediaModule()
			{}
			virtual ~MediaCapture(void) {}

		public:
			virtual int startCapture(const std::string& streamURL) = 0;
			virtual int stopCapture(void) = 0;
			int inputMediaData(MediaDataPtr mediaData) override
			{
				return ERR_NOT_SUPPORT;
			}
		};//class MediaCapture
	}//namespace multimedia
}//namespace framework

#endif//MEDIA_CAPTURE_H
