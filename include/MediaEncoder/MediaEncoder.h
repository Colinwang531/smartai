//
//		Copyright :					@2019, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2019-10-04
//		Description:					��ý��������������
//
//		History:						Author									Date														Description
//											������									2019-10-04										����
//

#ifndef MEDIA_ENCODER_H
#define MEDIA_ENCODER_H

#include "predef.h"

NS_BEGIN(encoder, 1)

class MediaEncoder
{
public:
	MediaEncoder(void);
	virtual ~MediaEncoder(void);

public:
	virtual int initialize(
		const unsigned short imageWidth = 1920, const unsigned short imageHeight = 1080) = 0;
	virtual void deinitialize(void) = 0;
	virtual int encode(
		const unsigned char* imageData = NULL, const unsigned long long imageBytes = 0,
		const unsigned short imageWidth = 1920, const unsigned short imageHeight = 1080) = 0;
	virtual void data(unsigned char*& outputData, unsigned long long& outputBytes) = 0;
};//class MediaEncoder

NS_END

#endif//MEDIA_ENCODER_H
