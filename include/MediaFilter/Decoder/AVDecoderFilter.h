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

class AVDecoderFilter : public MediaFilter
{
public:
	AVDecoderFilter(
		const MediaDataSubID subID = MediaDataSubID::MEDIA_DATA_SUB_ID_NONE);
	virtual ~AVDecoderFilter(void);

protected:
	int createNewFilter(void) override;
	int destroyFilter(void) override;
	int inputMediaData(MediaDataPtr mediaData) override;

private:
	const MediaDataSubID mediaDataSubID;
};//class AVDecoderFilter

NS_END

#endif//AV_DECODER_FILTER_H
