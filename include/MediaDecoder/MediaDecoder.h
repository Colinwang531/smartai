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

typedef enum class tagMediaDecodeType_t
{
	MEDIA_DECODE_TYPE_H2645 = 0,
	MEDIA_DECODE_TYPE_AAC,
	MEDIA_DECODE_TYPE_G722
}MediaDecodeType;

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
