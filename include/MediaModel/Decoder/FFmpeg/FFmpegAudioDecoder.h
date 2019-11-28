// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Audio decoder using FFMPEG library.
//

#ifndef FFMPEG_AUDIO_DECODER_H
#define FFMPEG_AUDIO_DECODER_H

#include "MediaModel/Decoder/MediaDecoder.h"

namespace framework
{
	namespace multimedia
	{
		class FFmpegAudioDecoder : public MediaDecoder
		{
		public:
			FFmpegAudioDecoder(void);
			virtual ~FFmpegAudioDecoder(void);

		protected:
			int inputMediaData(MediaDataPtr mediaData) override;
		};//class FFmpegVideoDecoder
	}//namespace multimedia
}//namespace framework

#endif//FFMPEG_AUDIO_DECODER_H
