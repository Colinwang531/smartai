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
		typedef enum class tagRendererType_t : unsigned char
		{
			RENDERER_TYPE_NONE = 0,
			RENDERER_TYPE_VIDEO,
			RENDERER_TYPE_AUDIO
		}RendererType;

		class AVRendererFilter : public MediaFilter
		{
		public:
			AVRendererFilter(const RendererType type = RendererType::RENDERER_TYPE_NONE);
			virtual ~AVRendererFilter(void);

		protected:
			int createNewFilter(void) override;
			int createNewModel(MediaDataPtr mediaData) override;

		private:
			const RendererType rendererType;
		};//class AVRendererFilter
	}//namespace multimedia
}//namespace framework

#endif//AV_RENDERER_FILTER_H
