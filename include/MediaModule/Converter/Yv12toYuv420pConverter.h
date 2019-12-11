// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// YV12 -> YUV420P converter filter.
//

#ifndef YV12_TO_YUV420P_CONVERTER_H
#define YV12_TO_YUV420P_CONVERTER_H

#include "MediaModule/Converter/MediaConverter.h"

namespace framework
{
	namespace multimedia
	{
		class Yv12toYuv420pConverter : public MediaConverter
		{
		public:
			Yv12toYuv420pConverter(void);
			~Yv12toYuv420pConverter(void);

		protected:
			int inputMediaData(MediaDataPtr mediaData) override;
		};//class Yv12toYuv420pConverter
	}//namespace multimedia
}//namespace framework

#endif//YV12_TO_YUV420P_CONVERTER_H
