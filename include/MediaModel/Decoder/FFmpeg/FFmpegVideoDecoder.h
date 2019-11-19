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
#include "MediaModel/Decoder/MediaDecoder.h"

NS_BEGIN(model, 1)

class FFmpegVideoDecoder : public MediaDecoder
{
public:
	FFmpegVideoDecoder(void);
	virtual ~FFmpegVideoDecoder(void);

protected:
	int inputMediaData(MediaDataPtr mediaData) override;

private:
	int initializeFFmpegAVCodec(MediaDataPtr mediaData);
	void deinitializeFFmpegAVCodec(void);
	int initializeFFmpegPacketAndFrame(MediaDataPtr mediaData);
	void deinitializeFFmpegPacketAndFrame(void);
	int decodeMediaData(MediaDataPtr mediaData);

private:
	AVCodecContext* codecctx;
	AVBufferRef* hwdevicectx;
// 	void* avcodecPacket;
// 	void* avcodecInputFrame;
// 	void* avcodecOutputFrame;
// 	void* swsContext;
// 	unsigned char* outputFrameData;
// 	int outputFrameDataBytes;
};//class FFmpegVideoDecoder

NS_END

#endif//FFMPEG_VIDEO_DECODER_H
