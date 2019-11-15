// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Video and audio demuxer filter class.
//

#ifndef AV_DEMUXER_FILTER_H
#define AV_DEMUXER_FILTER_H

#include "MediaFilter/SourceMediaFilter.h"

NS_BEGIN(filter, 1)

class AVDemuxerFilter : public SourceMediaFilter
{
public:
	AVDemuxerFilter(void);
	virtual ~AVDemuxerFilter(void);

protected:
	int createNewFilter(void) override;
	int destroyFilter(void) override;
};//class AVDemuxerFilter

NS_END

#endif//AV_DEMUXER_FILTER_H
