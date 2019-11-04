// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Abstract base class of pin.
//

#ifndef MEDIA_PIN_H
#define MEDIA_PIN_H

#include "boost/shared_ptr.hpp"
#include "boost/weak_ptr.hpp"
#include "MediaData/MediaData.h"
using MediaDataPtr = boost::shared_ptr<NS(media, 1)::MediaData>;

NS_BEGIN(pin, 1)

typedef enum class tagMediaPinMode_t
{
	MEDIA_PIN_INPUT = 0,
	MEDIA_PIN_OUTPUT
}MediaPinMode;

BOOST_STATIC_CONSTANT(std::string, VideoStreamOutputPinID = "VideoStreamOutputPinID");
BOOST_STATIC_CONSTANT(std::string, AudioStreamOutputPinID = "AudioStreamOutputPinID");

class MediaPin
{
protected:
	using MediaPinRef = boost::weak_ptr<MediaPin>;

public:
	MediaPin(const MediaPinMode mode = MediaPinMode::MEDIA_PIN_INPUT);
	virtual ~MediaPin(void);

public:
	virtual int connectPin(MediaPinRef inputPinRef);
	virtual int inputData(MediaDataPtr dataPtr);
	inline const MediaPinMode getMode(void) const
	{
		return mediaPinMode;
	}

private:
	const MediaPinMode mediaPinMode;
};//class MediaPin

NS_END

#endif//MEDIA_PIN_H
