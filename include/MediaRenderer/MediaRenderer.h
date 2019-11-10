// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Media stream renderer for displaying stream.
//

#ifndef MEDIA_RENDERER_H
#define MEDIA_RENDERER_H

#include "predef.h"

NS_BEGIN(renderer, 1)

class MediaRenderer
{
public:
	MediaRenderer(void);
	virtual ~MediaRenderer(void);

public:
	// If NULL == param, audio filter is created.
	// If NULL != param, video filter is created.
	virtual int createNewMediaRenderer(void* = NULL) = 0;
};//class MediaRenderer

NS_END

#endif//MEDIA_RENDERER_H
