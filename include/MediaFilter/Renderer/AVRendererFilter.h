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

namespace framework
{
	namespace multimedia
	{
		class AVRendererFilter : public MediaFilter
		{
		public:
			AVRendererFilter(void);
			virtual ~AVRendererFilter(void);

		public:
			inline void setHwnd(void* hwnd = NULL)
			{
				videoRendererWnd = hwnd;
			}

		protected:
			int createNewFilter(const std::string& streamURL) override;
			int createNewModel(MediaDataPtr mediaData) override;

		private:
			void* videoRendererWnd;
		};//class AVRendererFilter
	}//namespace multimedia
}//namespace framework

#endif//AV_RENDERER_FILTER_H
