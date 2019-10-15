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

#include "MediaPin.h"

NS_BEGIN(pin, 1)

class InputMediaPin : public MediaPin
{
public:
	InputMediaPin(void);
	virtual ~InputMediaPin(void);

public:
	virtual bool isInputPin(void) const;
};//class InputMediaPin

NS_END

#endif//INPUT_MEDIA_PIN_H
