// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Video decoder using FFMPEG library.
//

#ifndef FFMPEG_VIDEO_DECODER_H
#define FFMPEG_VIDEO_DECODER_H

extern "C"
{
#include "libavcodec/avcodec.h"
}
#include "MediaModule/MediaModule.h"

namespace framework
{
	namespace multimedia
	{
		class FFmpegVideoDecoder : public MediaModule
		{
		public:
			FFmpegVideoDecoder(void);
			virtual ~FFmpegVideoDecoder(void);

		protected:
			int inputMediaData(MediaDataPtr mediaData) override;

		private:
			int initializeVideoDecoder(MediaDataPtr mediaData);
			void deinitializeFFmpegAVCodec(void);
			int initializeFFmpegPacketAndFrame(MediaDataPtr mediaData);
			void deinitializeFFmpegPacketAndFrame(void);
			int decodeMediaData(MediaDataPtr mediaData);

		private:
			AVCodecContext* codecctx;
			AVBufferRef* hwdevicectx;
			void* avcodecPacket;
			void* avcodecInputFrame;
			void* avcodecOutputFrame;
			void* swsContext;
			unsigned char* outputFrameData;
			int outputFrameDataBytes;
		};//class FFmpegVideoDecoder
	}//namespace multimedia
}//namespace framework

#endif//FFMPEG_VIDEO_DECODER_H
