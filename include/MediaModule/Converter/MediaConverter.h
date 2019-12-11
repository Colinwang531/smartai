// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Abstract base class of data converting module.
//

#ifndef MEDIA_CONVERTER_H
#define MEDIA_CONVERTER_H

#include "MediaModule/MediaModule.h"

namespace framework
{
	namespace multimedia
	{
		class MediaConverter : public MediaModule
		{
		public:
			MediaConverter(void) : MediaModule()
			{}
			virtual ~MediaConverter(void) {}
		};//class MediaCapture
	}//namespace multimedia
}//namespace framework

#endif//MEDIA_CONVERTER_H
