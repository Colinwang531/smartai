// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Base class of inputing pin.
//

#ifndef INPUT_MEDIA_PIN_H
#define INPUT_MEDIA_PIN_H

#include "MediaFilter/MediaFilter.h"
using MediaFilterRef = boost::weak_ptr<NS(filter, 1)::MediaFilter>;

NS_BEGIN(pin, 1)

class InputMediaPin : public MediaPin
{
public:
	InputMediaPin(MediaFilterRef filterRef);
	virtual ~InputMediaPin(void);

public:
	int inputData(MediaDataPtr dataPtr) override;

private:
	MediaFilterRef parentMediaFilterRef;
};//class InputMediaPin

NS_END

#endif//INPUT_MEDIA_PIN_H
