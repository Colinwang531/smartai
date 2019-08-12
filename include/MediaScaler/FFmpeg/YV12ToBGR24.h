//
//		Copyright :					@2017, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2019-07-25
//		Description:					FFmpeg帧数据格式转换类
//
//		History:						Author									Date														Description
//											王科威									2019-07-25										创建
//

#ifndef YV12_TO_BGR24_H
#define YV12_TO_BGR24_H

extern "C"
{
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
}
#include "boost/config.hpp"
#include "MediaScaler/FrameScaler.h"

NS_BEGIN(scaler, 1)

class YV12ToBGR24 : public FrameScaler
{
public:
	YV12ToBGR24(void);
	virtual ~YV12ToBGR24(void);

public:
	const char* scale(
		const char* frame = NULL, const int frameBytes = 0, const int w = 0, const int h = 0) override;

private:
	int initialize(const int w = 0, const int h = 0);
	void deinitialize(void);
	int convertYV12ToYUV420P(
		const char* data = NULL, const int dataBytes = 0, const int w = 1920, const int h = 1080);

private:
	AVFrame* yv12FrameData;
	AVFrame* bgr24FrameData;
	char* yv12FrameDataBuffer;
	char* bgr24FrameDataBuffer;
	char* yuv420PFrameData;
	struct SwsContext* swsContext;
};//class YV12ToBGR24

NS_END

#endif//YV12_TO_BGR24_H
