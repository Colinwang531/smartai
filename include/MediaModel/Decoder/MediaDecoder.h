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

NS_BEGIN(model, 1)

class MediaDecoder : public MediaModel
{
public:
	MediaDecoder(void);
	virtual ~MediaDecoder(void);
};//class MediaDecoder

NS_END

#endif//MEDIA_DECODER_H
