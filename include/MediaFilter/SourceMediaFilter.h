// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Base class of source filter.
//

#ifndef SOURCE_MEDIA_FILTER_H
#define SOURCE_MEDIA_FILTER_H

#include "MediaFilter.h"

NS_BEGIN(filter, 1)

class SourceMediaFilter : public MediaFilter
{
public:
	SourceMediaFilter(void);
	virtual ~SourceMediaFilter(void);

protected:
	int inputMediaData(MediaDataPtr mediaData) override;
};//class SourceMediaFilter

NS_END

#endif//SOURCE_MEDIA_FILTER_H
