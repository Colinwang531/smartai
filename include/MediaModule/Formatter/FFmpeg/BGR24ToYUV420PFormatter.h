// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Base class of formatter for converting image form BGR24 to YUV420P.
//

#ifndef BGR24_TO_YUV420P_FORMATTER_H
#define BGR24_TO_YUV420P_FORMATTER_H

extern "C"
{
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
}
#include "MediaModel/MediaModel.h"

NS_BEGIN(model, 1)

class BGR24ToYUV420PFormatter : public MediaModel
{
public:
	BGR24ToYUV420PFormatter(void);
	virtual ~BGR24ToYUV420PFormatter(void);

protected:
	int inputMediaData(MediaDataPtr mediaData) override;

private:
	int initialize(
		const unsigned short imageWidth = 1920, const unsigned short imageHeight = 1080);
	void deinitialize(void);

private:
	unsigned char* yuv420pImage;
	int yuv420pImageBytes;
	AVFrame* yuv420pFrame;
	AVFrame* bgr24Frame;
	struct SwsContext* ctx;
};//class BGR24ToYUV420PFormatter

NS_END

#endif//BGR24_TO_YUV420P_FORMATTER_H
