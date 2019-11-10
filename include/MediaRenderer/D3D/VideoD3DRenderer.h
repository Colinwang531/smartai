// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Video display using D3D.
//

#ifndef VIDEO_D3D_RENDERER_H
#define VIDEO_D3D_RENDERER_H

#include "MediaRenderer/MediaRenderer.h"

NS_BEGIN(renderer, 1)

class VideoD3DRenderer : public MediaRenderer
{
public:
	VideoD3DRenderer(void);
	virtual ~VideoD3DRenderer(void);

protected:
	int createNewMediaRenderer(void* hwnd = NULL) override;
};//class VideoD3DRenderer

NS_END

#endif//VIDEO_D3D_RENDERER_H
