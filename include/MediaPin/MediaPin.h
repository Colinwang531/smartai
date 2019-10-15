//
//		Copyright :					@2019, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2019-10-13
//		Description:					多媒体针脚抽象基类
//
//		History:						Author									Date														Description
//											王科威									2019-10-13										创建
//

#ifndef MEDIA_PIN_H
#define MEDIA_PIN_H

#include "boost/shared_ptr.hpp"
#include "MediaData/MediaData.h"
using MediaDataPtr = boost::shared_ptr<NS(media, 1)::MediaData>;

NS_BEGIN(pin, 1)

class MediaPin
{
public:
	MediaPin(void);
	virtual ~MediaPin(void);

public:
	virtual int connectInputPin(boost::shared_ptr<MediaPin> inputPinPtr);
	virtual int inputData(MediaDataPtr dataPtr);
	virtual bool isInputPin(void) const;
	virtual bool isOutputPin(void) const;
};//class MediaPin

NS_END

#endif//MEDIA_PIN_H
