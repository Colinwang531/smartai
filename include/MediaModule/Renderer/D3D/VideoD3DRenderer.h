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

#include "MediaModule/Renderer/D3D/D3D/D3D.h"
using Direct3DPtr = boost::shared_ptr<framework::multimedia::D3D>;
#include "MediaModule/MediaModule.h"

namespace framework
{
	namespace multimedia
	{
		class VideoD3DRenderer : public MediaModule
		{
		public:
			VideoD3DRenderer(void* hwnd = NULL);
			virtual ~VideoD3DRenderer(void);

		protected:
			int inputMediaData(MediaDataPtr mediaData) override;

		private:
			int createNewMediaRenderer(MediaDataPtr mediaData);

		private:
			void* videoDisplayWnd;
			Direct3DPtr direct3dPtr;
		};//class VideoD3DRenderer
	}//namespace multimedia
}//namespace framework

#endif//VIDEO_D3D_RENDERER_H
