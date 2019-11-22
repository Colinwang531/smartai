// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Base class of formatter for converting video package.
//

#ifndef VIDEO_FORMATTER_FILTER_H
#define VIDEO_FORMATTER_FILTER_H

#include "MediaFilter/MediaFilter.h"

NS_BEGIN(filter, 1)

class VideoFormatterFilter : public MediaFilter
{
public:
	VideoFormatterFilter(void);
	virtual ~VideoFormatterFilter(void);

protected:
	int createNewFilter(void) override;
	int destroyFilter(void) override;
	int inputMediaData(MediaDataPtr mediaData) override;
};//class VideoFormatterFilter

NS_END

#endif//VIDEO_FORMATTER_FILTER_H
