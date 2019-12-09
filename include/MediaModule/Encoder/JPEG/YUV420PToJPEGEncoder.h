// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Base class of encoder for encoding JPEG picture.
//

#ifndef YUV420P_TO_JPEG_ENCODER_H
#define YUV420P_TO_JPEG_ENCODER_H

extern "C"
{
#include "libavformat/avformat.h"
}
#include "MediaModel/MediaModel.h"

NS_BEGIN(model, 1)

class YUV420PToJPEGEncoder : public MediaModel
{
public:
	YUV420PToJPEGEncoder(void);
	virtual ~YUV420PToJPEGEncoder(void);

protected:
	int inputMediaData(MediaDataPtr mediaData) override;

private:
	int initialize(
		const unsigned short imageWidth = 1920, const unsigned short imageHeight = 1080);
	void deinitialize(void);

private:
	AVFrame* yuv420pFrame;
	AVCodecContext* ctx;
};//class YUV420PToJPEGEncoder

NS_END

#endif//YUV420P_TO_JPEG_ENCODER_H

