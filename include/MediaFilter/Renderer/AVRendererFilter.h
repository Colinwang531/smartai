// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Renderer of video and audio for displaying stream.
//

#ifndef AV_RENDERER_FILTER_H
#define AV_RENDERER_FILTER_H

#include <windows.h>
#include "MediaFilter/MediaFilter.h"

NS_BEGIN(filter, 1)

class AVRendererFilter : public MediaFilter
{
public:
	// If NULL == hwnd, audio filter is created.
	// If NULL != hwnd, video filter is created.
	AVRendererFilter(const HWND hwnd = NULL);
	virtual ~AVRendererFilter(void);

protected:
	int createNewFilter(void) override;
	int destroyFilter(void) override;
	int inputMediaData(MediaDataPtr mediaData) override;

private:
	const HWND videoDisplayWnd;
};//class AVRendererFilter

NS_END

#endif//AV_RENDERER_FILTER_H
