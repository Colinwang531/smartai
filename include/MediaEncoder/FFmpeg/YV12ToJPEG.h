//
//		Copyright :					@2019, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2019-07-31
//		Description:					FFMPEG库JPEG编码器抽象基类
//
//		History:						Author									Date												Description
//											王科威									2019-07-31								创建
//

#ifndef YV12_TO_JPEG_H
#define YV12_TO_JPEG_H

extern "C"
{
#include "libavformat/avformat.h"
}
#include "boost/function.hpp"
#include "MediaEncoder/MediaEncoder.h"

NS_BEGIN(encoder, 1)

class YV12ToJPEG : public MediaEncoder
{
public:
	YV12ToJPEG(void);
	virtual ~YV12ToJPEG(void);

protected:
	int encode(char*& jpegData, int& jpegBytes,
		const char* data = NULL, const int dataBytes = 0, 
		const int w = 1920, const int h = 1080) override;

private:
	int encodeJPEG(char*& jpegData, int& jpegBytes);
	int convertYV12ToYUV420P(
		const char* data = NULL, const int dataBytes = 0, const int w = 1920, const int h = 1080);

private:
	bool openAVCodec;
	char* yuv420PFrameData;
	AVCodec* avCodec;
	AVCodecContext* avCodecCtx;
};//class YV12ToJPEG

NS_END

#endif//YV12_TO_JPEG_H

