//
//		Copyright :					@2017, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2017-12-11
//		Description:					��ý�������������࣬���н����������Լ̳��ڴ�
//
//		History:						Author									Date														Description
//											������									2017-12-11										����
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
	virtual int encode(char*& jpegData, int& jpegBytes,
		const char* data = NULL, const int dataBytes = 0, const int w = 1920, const int h = 1080);
};//class MediaEncoder

NS_END

#endif//MEDIA_ENCODER_H
