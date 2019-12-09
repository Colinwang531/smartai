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

#include "MediaModel/Renderer/D3D/D3D/D3D.h"
using Direct3DPtr = boost::shared_ptr<framework::multimedia::D3D>;
#include "MediaModel/Renderer/MediaRenderer.h"

namespace framework
{
	namespace multimedia
	{
		class VideoD3DRenderer : public MediaRenderer
		{
		public:
			VideoD3DRenderer(const HWND hwnd = NULL);
			virtual ~VideoD3DRenderer(void);

		protected:
			int inputMediaData(MediaDataPtr mediaData) override;

		private:
			int createNewMediaRenderer(MediaDataPtr mediaData);

		private:
			const HWND videoDisplayWnd;
			Direct3DPtr direct3dPtr;
		};//class VideoD3DRenderer
	}//namespace multimedia
}//namespace framework

#endif//VIDEO_D3D_RENDERER_H
