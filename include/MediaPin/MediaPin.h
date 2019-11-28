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

#include <string>
#include "boost/shared_ptr.hpp"
#include "boost/weak_ptr.hpp"

static const std::string VideoStreamInputPinID = "VideoStreamInputPinID";
static const std::string AudioStreamInputPinID = "AudioStreamInputPinID";
static const std::string VideoStreamOutputPinID = "VideoStreamOutputPinID";
static const std::string AudioStreamOutputPinID = "AudioStreamOutputPinID";

namespace framework
{
	namespace multimedia
	{
		class MediaData;
		using MediaDataPtr = boost::shared_ptr<MediaData>;

		class MediaPin
		{
		protected:
			using MediaPinRef = boost::weak_ptr<MediaPin>;

		public:
			MediaPin(void);
			virtual ~MediaPin(void);

		public:
			virtual int connectPin(MediaPinRef pin) = 0;
			virtual int inputMediaData(MediaDataPtr mediaData) = 0;
			virtual bool isInputPin(void) const
			{
				return false;
			}
			virtual bool isOutputPin(void) const
			{
				return false;
			}
		};//class MediaPin
	}//namespace multimedia
}//namespace framework

#endif//MEDIA_PIN_H
