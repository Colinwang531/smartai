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

BOOST_STATIC_CONSTANT(std::string, MediaStreamInputPinID = "MediaStreamInputPinID");
BOOST_STATIC_CONSTANT(std::string, VideoStreamInputPinID = "VideoStreamInputPinID");
BOOST_STATIC_CONSTANT(std::string, AudioStreamInputPinID = "AudioStreamInputPinID");
BOOST_STATIC_CONSTANT(std::string, VideoStreamOutputPinID = "VideoStreamOutputPinID");
BOOST_STATIC_CONSTANT(std::string, AudioStreamOutputPinID = "AudioStreamOutputPinID");

class MediaPin
{
public:
	MediaPin(void);
	virtual ~MediaPin(void);

public:
	virtual int connectPin(boost::weak_ptr<MediaPin> inputPinRef);
	virtual int inputData(MediaDataPtr dataPtr);
	virtual bool isInputPin(void) const
	{
		return false;
	}
	virtual bool isOutputPin(void) const
	{
		return false;
	}
};//class MediaPin

NS_END

using MediaPinRef = boost::weak_ptr<NS(pin, 1)::MediaPin>;

#endif//MEDIA_PIN_H
