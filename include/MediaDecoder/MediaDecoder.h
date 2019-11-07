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

#include "predef.h"

NS_BEGIN(decoder, 1)

class MediaDecoder
{
public:
	MediaDecoder(void);
	virtual ~MediaDecoder(void);

public:
	// @ Deprecated
	virtual int decode(
		const unsigned char* frameData = NULL, const int frameBytes = 0) = 0;
};//class MediaDecoder

NS_END

#endif//MEDIA_DECODER_H
