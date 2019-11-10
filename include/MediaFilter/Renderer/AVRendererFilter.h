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

#include "MediaFilter/MediaFilter.h"
#include "MediaRenderer/MediaRenderer.h"
using MediaRendererPtr = boost::shared_ptr<NS(renderer, 1)::MediaRenderer>;

NS_BEGIN(filter, 1)

class AVRendererFilter : public MediaFilter
{
public:
	AVRendererFilter(void);
	virtual ~AVRendererFilter(void);

public:
	// If NULL == hwnd, audio filter is created.
	// If NULL != hwnd, video filter is created.
	int createNewMediaRenderer(void* hwnd = NULL);
	int inputMediaData(MediaDataPtr mediaData) override;

private:
	int createNewInputAndOutputPin(void* hwnd = NULL);

protected:
	MediaRendererPtr mediaRendererPtr;
};//class AVRendererFilter

NS_END

#endif//AV_RENDERER_FILTER_H
