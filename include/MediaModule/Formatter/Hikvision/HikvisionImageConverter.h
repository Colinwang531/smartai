//
//		Copyright :					@2019, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2019-07-25
//		Description:					海康实时流视频帧转换类
//
//		History:						Author									Date														Description
//											王科威									2019-07-25										创建
//

#ifndef HIKVISION_IMAGE_CONVERTER_H
#define HIKVISION_IMAGE_CONVERTER_H

#include "boost/shared_ptr.hpp"
#include "MediaConverter/YUV420PToBGR24Converter.h"

NS_BEGIN(converter, 1)

class HikvisionImageConverter final : public YUV420PToBGR24Converter
{
public:
	HikvisionImageConverter(void);
	~HikvisionImageConverter(void);

protected:
	int initialize(
		const unsigned short imageWidth = 1920, const unsigned short imageHeight = 1080);
	void deinitialize(void);
	const unsigned char* convert(
		const unsigned char* imageData = NULL, const unsigned long long imageBytes = 0,
		const unsigned short imageWidth = 1920, const unsigned short imageHeight = 1080) override;

private:
	boost::shared_ptr<MediaConverter> yv12ToYUV420PConverterPtr;
};//class HikvisionImageConverter

NS_END

#endif//YV12_TO_BGR24_H
