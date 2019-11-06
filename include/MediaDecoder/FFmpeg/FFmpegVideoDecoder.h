// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Video decoder using FFMPEG.
//

#ifndef FFMPEG_VIDEO_DECODER_H
#define FFMPEG_VIDEO_DECODER_H

#include "MediaDecoder/MediaDecoder.h"

NS_BEGIN(decoder, 1)

class FFmpegVideoDecoder : public MediaDecoder
{
public:
	FFmpegVideoDecoder(void);
	virtual ~FFmpegVideoDecoder(void);

public:
	// @ Deprecated
	int decode(
		const unsigned char* frameData = NULL, const int frameBytes = 0) override;
};//class FFmpegVideoDecoder

NS_END

#endif//FFMPEG_VIDEO_DECODER_H
