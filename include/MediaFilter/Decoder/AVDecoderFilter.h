// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Video and audio decoder filter class.
//

#ifndef AV_DECODER_FILTER_H
#define AV_DECODER_FILTER_H

#include "MediaFilter/MediaFilter.h"

namespace framework
{
	namespace multimedia
	{
		class AVDecoderFilter : public MediaFilter
		{
		public:
			AVDecoderFilter(void);
			virtual ~AVDecoderFilter(void);

		protected:
			int createNewFilter(const std::string& streamURL) override;
			int createNewModel(MediaDataPtr mediaData) override;
		};//class AVDecoderFilter
	}//namespace multimedia
}//namespace framework

#endif//AV_DECODER_FILTER_H
