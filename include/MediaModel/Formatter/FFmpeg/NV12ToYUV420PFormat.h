//
//		Copyright :					@2019, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2019-07-25
//		Description:					FFmpeg YV12->YUV420P转换类
//
//		History:						Author									Date														Description
//											王科威									2019-07-25										创建
//

#ifndef YV12_TO_YUV420P_CONVERTER_H
#define YV12_TO_YUV420P_CONVERTER_H

#include "MediaConverter/MediaConverter.h"

NS_BEGIN(converter, 1)

class YV12ToYUV420PConverter : public MediaConverter
{
public:
	YV12ToYUV420PConverter(void);
	virtual ~YV12ToYUV420PConverter(void);

protected:
	int initialize(
		const unsigned short imageWidth = 1920, const unsigned short imageHeight = 1080) override;
	void deinitialize(void) override;
	const unsigned char* convert(
		const unsigned char* imageData = NULL, const unsigned long long imageBytes = 0,
		const unsigned short imageWidth = 1920, const unsigned short imageHeight = 1080) override;

private:
	unsigned char* outputFrameData;
};//class YV12ToYUV420PConverter

NS_END

#endif//YV12_TO_YUV420P_CONVERTER_H
