// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Live stream capture.
//

#ifndef LIVE_STREAM_CAPTURE_FILTER_H
#define LIVE_STREAM_CAPTURE_FILTER_H

#include "MediaFilter/SourceMediaFilter.h"

NS_BEGIN(filter, 1)

class LivestreamCaptureFilter : public SourceMediaFilter
{
public:
	LivestreamCaptureFilter(void);
	virtual ~LivestreamCaptureFilter(void);

protected:
	int createNewFilter(void) override;
	int destroyFilter(void) override;
};//class LivestreamCaptureFilter

NS_END

#endif//LIVE_STREAM_CAPTURE_FILTER_H
