// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Abstract base class of filter.
//

#ifndef MEDIA_FILTER_H
#define MEDIA_FILTER_H

#include "boost/enable_shared_from_this.hpp"
#include "DataStruct/UnorderedMap.h"

static const std::string AVMediaDataCaptureFilterID = "AVMediaDataCaptureFilterID";
static const std::string AVMediaPlayControlFilterID = "AVMediaPlayControlFilterID";
static const std::string AVMediaImageFormatFilterID = "AVMediaImageFormatFilterID";
static const std::string AVMediaSDKDecoderFilterID = "AVMediaSDKDecoderFilterID";
static const std::string AVMediaVideoDecoderFilterID = "AVMediaVideoDecoderFilterID";
static const std::string AVMediaAudioDecoderFilterID = "AVMediaAudioDecoderFilterID";
static const std::string AVMediaVideoRendererFilterID = "AVMediaVideoRendererFilterID";
static const std::string AVMediaSoundPlayerFilterID = "AVMediaSoundPlayerFilterID";
static const std::string AVMediaDataCallbackFilterID = "AVMediaDataCallbackFilterID";

namespace framework
{
	namespace multimedia
	{
		typedef enum tagMediaPinFlag_t : unsigned char
		{
			MEDIA_PIN_FLAG_NONE = 0,
			MEDIA_PIN_FLAG_VIDEO,
			MEDIA_PIN_FLAG_AUDIO
		}MediaPinFlag;

		class MediaData;
		class MediaModel;
		class MediaPin;
		using MediaDataPtr = boost::shared_ptr<MediaData>;
		using MediaPinPtr = boost::shared_ptr<MediaPin>;
		using MediaPinRef = boost::weak_ptr<MediaPin>;
		using MediaPinGroup = UnorderedMap<const std::string, MediaPinPtr>;
		using MediaModelPtr = boost::shared_ptr<MediaModel>;
		using MediaModelRef = boost::weak_ptr<MediaModel>;

		class MediaFilter : public boost::enable_shared_from_this<MediaFilter>
		{
		public:
			MediaFilter(void);
			virtual ~MediaFilter(void);

		public:
			//
			// @iflag : video=1, audio=2, mix=3
			// @oflag : video=1, audio=2, mix=3
			//
			int createNewFilter(
				const unsigned char iflag = MEDIA_PIN_FLAG_NONE, const unsigned char oflag = MEDIA_PIN_FLAG_NONE);
			int destroyFilter(void);
			virtual int inputMediaData(MediaDataPtr mediaData);
			virtual bool isSourceFilter(void) const
			{
				return false;
			}
			virtual bool isTargetFilter(void) const
			{
				return false;
			}
			inline MediaPinRef queryMediaPinByID(const std::string& pinID)
			{
				return mediaPinGroup.at(pinID);
			}

		protected:
			virtual int createNewModel(MediaDataPtr mediaData);
			int createNewInputPin(const unsigned char iflag = 0);
			int createNewOutputPin(const unsigned char oflag = 0);
			int postInputMediaData(MediaDataPtr mediaData);

		protected:
			MediaPinGroup mediaPinGroup;
			MediaModelPtr mediaModelPtr;
		};//class MediaFilter
	}//namespace multimedia
}//namespace framework

#endif//MEDIA_FILTER_H
