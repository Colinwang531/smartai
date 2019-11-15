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

#include "MediaModel/MediaModel.h"

NS_BEGIN(model, 1)

class MediaRenderer : public MediaModel
{
public:
	MediaRenderer(void);
	virtual ~MediaRenderer(void);
};//class MediaRenderer

NS_END

#endif//MEDIA_RENDERER_H
