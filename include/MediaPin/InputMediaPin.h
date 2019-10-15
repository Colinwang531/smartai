//
//		Copyright :					@2019, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2019-10-13
//		Description:					多媒体输入针脚类
//
//		History:						Author									Date														Description
//											王科威									2019-10-13										创建
//

#ifndef INPUT_MEDIA_PIN_H
#define INPUT_MEDIA_PIN_H

#include "MediaFilter/MediaFilter.h"
using MediaFilterPtr = boost::shared_ptr<NS(filter, 1)::MediaFilter>;
#include "MediaPin.h"

NS_BEGIN(pin, 1)

class InputMediaPin : public MediaPin
{
public:
	InputMediaPin(MediaFilterPtr filterPtr);
	virtual ~InputMediaPin(void);

public:
	int connectInputPin(boost::shared_ptr<MediaPin> inputPinPtr) override;
	int inputData(MediaDataPtr dataPtr) override;
	virtual bool isInputPin(void) const;

private:
	MediaFilterPtr mediaFilterPtr;
};//class InputMediaPin

NS_END

#endif//INPUT_MEDIA_PIN_H
