//
//		Copyright :					@2017, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2017-12-11
//		Description:					多媒体解码器抽象基类
//
//		History:						Author									Date														Description
//											王科威									2017-12-11										创建
//

#ifndef MEDIA_DECODER_H
#define MEDIA_DECODER_H

#include "predef.h"

NS_BEGIN(decoder, 1)

class MediaDecoder
{
public:
	MediaDecoder(void);
	virtual ~MediaDecoder(void);

public:
	virtual int decode(
		const unsigned char* frameData = NULL, const int frameBytes = 0) = 0;
};//class MediaDecoder

NS_END

#endif//MEDIA_DECODER_H
