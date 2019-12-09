// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Base class of video and audio decoder.
//

#ifndef MEDIA_DECODER_H
#define MEDIA_DECODER_H

#include "MediaModel/MediaModel.h"

namespace framework
{
	namespace multimedia
	{
		class MediaDecoder : public MediaModel
		{
		public:
			MediaDecoder(void);
			virtual ~MediaDecoder(void);
		};//class MediaDecoder
	}//namespace multimedia
}//namespace framework

#endif//MEDIA_DECODER_H
