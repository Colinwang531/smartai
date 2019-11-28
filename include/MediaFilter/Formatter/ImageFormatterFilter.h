// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Base class of formatter for converting video package.
//

#ifndef IMAGE_FORMATTER_FILTER_H
#define IMAGE_FORMATTER_FILTER_H

#include "MediaFilter/MediaFilter.h"

namespace framework
{
	namespace multimedia
	{
		class ImageFormatterFilter : public MediaFilter
		{
		public:
			ImageFormatterFilter(void);
			virtual ~ImageFormatterFilter(void);

		protected:
			int createNewFilter(
				const MediaStreamID mediaStreamID = MediaStreamID::MEDIA_STREAM_ID_AV) override;
			int inputMediaData(MediaDataPtr mediaData) override;
		};//class ImageFormatterFilter
	}//namespace multimedia
}//namespace framework

#endif//IMAGE_FORMATTER_FILTER_H
