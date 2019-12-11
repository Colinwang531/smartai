// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// NV12 -> YUV420P converter filter using FFmpeg library.
//

#ifndef NV12_TO_YUV420P_CONVERTER_H
#define NV12_TO_YUV420P_CONVERTER_H

#include "MediaModule/Converter/MediaConverter.h"

namespace framework
{
	namespace multimedia
	{
		class YV12ToYUV420PConverter : public MediaConverter
		{
		public:
			YV12ToYUV420PConverter(void);
			virtual ~YV12ToYUV420PConverter(void);

		protected:
			int inputMediaData(MediaDataPtr mediaData) override;
// 			int initialize(
// 				const unsigned short imageWidth = 1920, const unsigned short imageHeight = 1080) override;
// 			void deinitialize(void) override;
// 			const unsigned char* convert(
// 				const unsigned char* imageData = NULL, const unsigned long long imageBytes = 0,
// 				const unsigned short imageWidth = 1920, const unsigned short imageHeight = 1080) override;

		private:
			unsigned char* outputFrameData;
		};//class YV12ToYUV420PConverter
	}//namespace multimedia
}//namespace framework

#endif//NV12_TO_YUV420P_CONVERTER_H
