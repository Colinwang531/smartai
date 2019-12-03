// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Image converter using FFmpeg library.
//

#ifndef FFMPEG_IMAGE_FORMATTER_H
#define FFMPEG_IMAGE_FORMATTER_H

extern "C"
{
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
}
#include "MediaModel/Formatter/MediaFormatter.h"

namespace framework
{
	namespace multimedia
	{
		class FFmpegImageFormatter : public MediaFormatter
		{
		public:
			FFmpegImageFormatter(
				const MediaDataSubID mediaDataSubID = MediaDataSubID::MEDIA_DATA_SUB_ID_NONE);
			virtual ~FFmpegImageFormatter(void);

		protected:
			int inputMediaData(MediaDataPtr mediaData) override;

		protected:
			int createNewImageFormatter(MediaDataPtr mediaData);
			void destroyImageFormatter(void);
			int scaleImageData(MediaDataPtr mediaData);

		protected:
			const MediaDataSubID oformat;
			struct SwsContext* ctx;
			AVFrame* iframe;
			AVFrame* oframe;
			unsigned char* obuffer;
			int obufferBytes;
		};//class FFmpegImageFormatter
	}//namespace multimedia
}//namespace framework

#endif//FFMPEG_IMAGE_FORMATTER_H
