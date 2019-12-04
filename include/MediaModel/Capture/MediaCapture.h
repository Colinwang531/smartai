// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Base class of live stream capture.
//

#ifndef MEDIA_CAPTURE_H
#define MEDIA_CAPTURE_H

#include "MediaModel/MediaModel.h"

namespace framework
{
	namespace multimedia
	{
		class MediaDevice;
		using MediaDevicePtr = boost::shared_ptr<MediaDevice>;

		class MediaCapture : public MediaModel
		{
		public:
			MediaCapture(void);
			virtual ~MediaCapture(void);

		protected:
			int inputMediaData(MediaDataPtr mediaData) override;
			virtual int openStream(const std::string& streamUrl) = 0;
			virtual int closeStream(void) = 0;

		protected:
			// Any hardware.
			MediaDevicePtr mediaDevicePtr;
		};//class MediaCapture
	}//namespace multimedia
}//namespace framework

#endif//MEDIA_CAPTURE_H
