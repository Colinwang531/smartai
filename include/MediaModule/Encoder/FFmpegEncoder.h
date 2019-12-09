//
//		Copyright :					@2019, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2019-07-31
//		Description:					FFMPEG编码器类
//
//		History:						Author									Date												Description
//											王科威									2019-07-31								创建
//

#ifndef FFMPEG_ENCODER_H
#define FFMPEG_ENCODER_H

extern "C"
{
#include "libavformat/avformat.h"
}
#include "MediaEncoder/MediaEncoder.h"

NS_BEGIN(encoder, 1)

class FFmpegEncoder : public MediaEncoder
{
public:
	FFmpegEncoder(void);
	virtual ~FFmpegEncoder(void);

protected:
	int initialize(
		const unsigned short imageWidth = 1920, const unsigned short imageHeight = 1080) override;
	void deinitialize(void) override;
	int encode(
		const unsigned char* imageData = NULL, const unsigned long long imageBytes = 0,
		const unsigned short imageWidth = 1920, const unsigned short imageHeight = 1080) override;
	void data(unsigned char*& outputData, unsigned long long& outputBytes) override;

protected:
	unsigned char* outputImageData;
	unsigned long long outputImageDataBytes;
	AVCodec* codec;
	AVCodecContext* ctx;
};//class YV12ToJPEG

NS_END

#endif//FFMPEG_ENCODER_H

