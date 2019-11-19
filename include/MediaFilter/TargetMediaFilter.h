// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Base class of target filter.
//

#ifndef TARGET_MEDIA_FILTER_H
#define TARGET_MEDIA_FILTER_H

#include "MediaFilter.h"

NS_BEGIN(filter, 1)

class TargetMediaFilter : public MediaFilter
{
public:
	TargetMediaFilter(void);
	virtual ~TargetMediaFilter(void);
};//class TargetMediaFilter

NS_END

#endif//TARGET_MEDIA_FILTER_H
