//
//		Copyright :					@2019, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2019-07-31
//		Description:					FFMPEG YUV420P->JPEG编码器类
//
//		History:						Author									Date												Description
//											王科威									2019-07-31								创建
//

#ifndef YUV420P_TO_JPEG_ENCODER_H
#define YUV420P_TO_JPEG_ENCODER_H

#include "MediaEncoder/FFmpegEncoder.h"

NS_BEGIN(encoder, 1)

class YUV420PToJPEGEncoder : public FFmpegEncoder
{
public:
	YUV420PToJPEGEncoder(void);
	virtual ~YUV420PToJPEGEncoder(void);

protected:
	int initialize(
		const unsigned short imageWidth = 1920, const unsigned short imageHeight = 1080) override;
	void deinitialize(void) override;
	int encode(
		const unsigned char* imageData = NULL, const unsigned long long imageBytes = 0,
		const unsigned short imageWidth = 1920, const unsigned short imageHeight = 1080) override;

private:
	AVFrame* inputAVFrame;
};//class YUV420PToJPEGEncoder

NS_END

#endif//YUV420P_TO_JPEG_ENCODER_H

