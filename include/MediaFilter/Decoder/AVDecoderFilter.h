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
#include "MediaDecoder/MediaDecoder.h"
using MediaDecoderPtr = boost::shared_ptr<NS(decoder, 1)::MediaDecoder>;

NS_BEGIN(filter, 1)

class AVDecoderFilter : public MediaFilter
{
public:
	AVDecoderFilter(void);
	virtual ~AVDecoderFilter(void);

public:
	int createNewMediaDecoder(
		const MediaStreamID streamID = MediaStreamID::MEDIA_STREAM_ID_NONE);

protected:
	int inputMediaData(MediaDataPtr mediaData) override;

private:
	int createNewInputAndOutputPin(
		const MediaStreamID streamID = MediaStreamID::MEDIA_STREAM_ID_NONE);

protected:
	MediaDecoderPtr mediaDecoderPtr;
};//class AVDecoderFilter

NS_END

#endif//AV_DECODER_FILTER_H
