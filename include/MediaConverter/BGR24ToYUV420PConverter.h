//
//		Copyright :					@2019, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2019-07-25
//		Description:					FFmpeg YUV420P->BGR24转换类
//
//		History:						Author									Date														Description
//											王科威									2019-07-25										创建
//

#ifndef BGR24_TO_YUV420P_CONVERTER_H
#define BGR24_TO_YUV420P_CONVERTER_H

#include "MediaConverter/FFmpegConverter.h"

NS_BEGIN(converter, 1)

class BGR24ToYUV420PConverter : public FFmpegConverter
{
public:
	BGR24ToYUV420PConverter(void);
	virtual ~BGR24ToYUV420PConverter(void);

protected:
	int initialize(
		const unsigned short imageWidth = 1920, const unsigned short imageHeight = 1080);
	void deinitialize(void);
	const unsigned char* convert(
		const unsigned char* imageData = NULL, const unsigned long long imageBytes = 0,
		const unsigned short imageWidth = 1920, const unsigned short imageHeight = 1080) override;
};//class YUV420PToBGR24Converter

NS_END

#endif//BGR24_TO_YUV420P_CONVERTER_H
