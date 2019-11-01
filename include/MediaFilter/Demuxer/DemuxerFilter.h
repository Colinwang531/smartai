// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Demuxer filter.
//

#ifndef DEMUXER_FILTER_H
#define DEMUXER_FILTER_H

#include "MediaFilter/SourceMediaFilter.h"

NS_BEGIN(filter, 1)

class DemuxerFilter : public SourceMediaFilter
{
public:
	DemuxerFilter(void);
	virtual ~DemuxerFilter(void);
};//class DemuxerFilter

NS_END

#endif//DEMUXER_FILTER_H
