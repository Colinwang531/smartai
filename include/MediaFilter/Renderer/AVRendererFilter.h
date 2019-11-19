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

NS_BEGIN(filter, 1)

typedef enum class tagAVRendererType_t
{
	AV_RENDERER_TYPE_NONE = 0,
	AV_RENDERER_TYPE_VIDEO,
	AV_RENDERER_TYPE_AUDIO
}AVRendererType;

class AVRendererFilter : public MediaFilter
{
public:
	AVRendererFilter(
		const AVRendererType type = AVRendererType::AV_RENDERER_TYPE_NONE);
	virtual ~AVRendererFilter(void);

protected:
	int createNewFilter(void) override;
	int destroyFilter(void) override;
	int inputMediaData(MediaDataPtr mediaData) override;

private:
	const AVRendererType rendererType;
};//class AVRendererFilter

NS_END

#endif//AV_RENDERER_FILTER_H
