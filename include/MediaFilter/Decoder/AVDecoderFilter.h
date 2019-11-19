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

NS_BEGIN(filter, 1)

typedef enum class tagAVDecoderType_t
{
	AV_DECODER_TYPE_NONE = 0,
	AV_DECODER_TYPE_VIDEO,
	AV_DECODER_TYPE_AUDIO
}AVDecoderType;

class AVDecoderFilter : public MediaFilter
{
public:
	AVDecoderFilter(
		const AVDecoderType type = AVDecoderType::AV_DECODER_TYPE_NONE);
	virtual ~AVDecoderFilter(void);

protected:
	int createNewFilter(void) override;
	int destroyFilter(void) override;
	int inputMediaData(MediaDataPtr mediaData) override;

private:
	const AVDecoderType decoderType;
};//class AVDecoderFilter

NS_END

#endif//AV_DECODER_FILTER_H
