// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Base class of outputting pin.
//

#ifndef OUTPUT_MEDIA_PIN_H
#define OUTPUT_MEDIA_PIN_H

#include "DataStruct/Vector.h"
#include "MediaPin.h"

NS_BEGIN(pin, 1)

class OutputMediaPin : public MediaPin
{
public:
	OutputMediaPin(void);
	virtual ~OutputMediaPin(void);

public:
	int connectPin(MediaPinRef inputPinRef) override;
	int inputData(MediaDataPtr dataPtr) override;

private:
	Vector<MediaPinRef> inputPinRefGroup;
};//class OutputMediaPin

NS_END

#endif//OUTPUT_MEDIA_PIN_H
