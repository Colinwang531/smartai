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

#include "Direct3d/D3D.h"
using Direct3DPtr = boost::shared_ptr<NS(d3d, 1)::D3D>;
#include "MediaModel/Renderer/MediaRenderer.h"

NS_BEGIN(model, 1)

class VideoD3DRenderer : public MediaRenderer
{
public:
	VideoD3DRenderer(void);
	virtual ~VideoD3DRenderer(void);

public:
	inline void setVideoDisplayWnd(const HWND hwnd = NULL)
	{
		videoDisplayWnd = hwnd;
	}

protected:
	int inputMediaData(MediaDataPtr mediaData) override;

private:
	int createNewMediaRenderer(MediaDataPtr mediaData);

private:
	HWND videoDisplayWnd;
	Direct3DPtr direct3dPtr;
};//class VideoD3DRenderer

NS_END

#endif//VIDEO_D3D_RENDERER_H
