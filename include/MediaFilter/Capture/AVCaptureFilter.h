// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Capture of video and audio for feeding data to caller.
//

#ifndef AV_CAPTURE_FILTER_H
#define AV_CAPTURE_FILTER_H

#include "MediaFilter/TargetMediaFilter.h"

NS_BEGIN(filter, 1)

class AVCaptureFilter : public TargetMediaFilter
{
protected:
	typedef void (CALLBACK* AVMediaDataCaptureCallback)(const char*, const int);

public:
	AVCaptureFilter(void* callback = NULL);
	virtual ~AVCaptureFilter(void);

protected:
	int createNewFilter(void) override;
	int destroyFilter(void) override;
	int inputMediaData(MediaDataPtr mediaData) override;

private:
	AVMediaDataCaptureCallback avMediaDataCaptureCallback;
};//class AVCaptureFilter

NS_END

#endif//AV_CAPTURE_FILTER_H
