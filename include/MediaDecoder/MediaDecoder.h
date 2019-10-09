//
//		Copyright :					@2017, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2017-12-11
//		Description:					��ý��������������
//
//		History:						Author									Date														Description
//											������									2017-12-11										����
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
