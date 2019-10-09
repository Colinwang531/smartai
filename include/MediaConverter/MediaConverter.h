//
//		Copyright :					@2019, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2019-07-25
//		Description:					帧格式转换抽象基类
//
//		History:						Author									Date														Description
//											王科威									2019-07-25										创建
//

#ifndef MEDIA_CONVERTER_H
#define MEDIA_CONVERTER_H

#include "predef.h"

NS_BEGIN(converter, 1)

class MediaConverter
{
public:
	MediaConverter(void);
	virtual ~MediaConverter(void);

public:
	virtual int initialize(
		const unsigned short imageWidth = 1920, const unsigned short imageHeight = 1080) = 0;
	virtual void deinitialize(void) = 0;
	virtual const unsigned char* convert(
		const unsigned char* imageData = NULL, const unsigned long long imageBytes = 0, 
		const unsigned short imageWidth = 1920, const unsigned short imageHeight = 1080) = 0;
};//class MediaConverter

NS_END

#endif//MEDIA_CONVERTER_H
