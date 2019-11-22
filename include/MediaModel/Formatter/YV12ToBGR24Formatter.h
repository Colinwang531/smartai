// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Base class of formatter for converting YV12 to BGR24.
//

#ifndef YV12_TO_BGR24_FORMATTER_H
#define YV12_TO_BGR24_FORMATTER_H

extern "C"
{
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
}
#include "MediaModel/MediaModel.h"

NS_BEGIN(model, 1)

class YV12ToBGR24Formatter : public MediaModel
{
public:
	YV12ToBGR24Formatter(void);
	virtual ~YV12ToBGR24Formatter(void);

protected:
	int initialize(
		const unsigned short imageWidth = 1920, const unsigned short imageHeight = 1080);
	void deinitialize(void);
	int inputMediaData(MediaDataPtr mediaData) override;

private:
	unsigned char* yuv420pImage;
	unsigned char* bgr24Image;
	int yuv420pImageBytes;
	int bgr24ImageBytes;
	AVFrame* yuv420pFrame;
	AVFrame* bgr24Frame;
	struct SwsContext* ctx;
};//class YV12ToBGR24Formatter

NS_END

#endif//YV12_TO_BGR24_FORMATTER_H
