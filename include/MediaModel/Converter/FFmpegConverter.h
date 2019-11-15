//
//		Copyright :					@2019, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2019-07-25
//		Description:					FFmpeg֡��ʽת����
//
//		History:						Author									Date														Description
//											������									2019-07-25										����
//

#ifndef FFMPEG_CONVERTER_H
#define FFMPEG_CONVERTER_H

extern "C"
{
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
}
#include "MediaConverter/MediaConverter.h"

NS_BEGIN(converter, 1)

class FFmpegConverter : public MediaConverter
{
public:
	FFmpegConverter(const AVPixelFormat src, const AVPixelFormat target);
	virtual ~FFmpegConverter(void);

protected:
	int initialize(
		const unsigned short imageWidth = 1920, const unsigned short imageHeight = 1080) override;
	void deinitialize(void) override;
	const unsigned char* convert(
		const unsigned char* imageData = NULL, const unsigned long long imageBytes = 0,
		const unsigned short imageWidth = 1920, const unsigned short imageHeight = 1080) override;

protected:
	AVFrame* inputAVFrame;
	AVFrame* outputAVFrame;
	unsigned char* inputFrameData;
	unsigned char* outputFrameData;
	struct SwsContext* swsContext;
	const AVPixelFormat sourceFormat;
	const AVPixelFormat targetFormat;
};//class FFmpegConverter

NS_END

#endif//FFMPEG_CONVERTER_H
