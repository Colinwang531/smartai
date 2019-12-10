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
		class MediaData;
		class MediaModule;
		class MediaPin;
		using MediaDataPtr = boost::shared_ptr<MediaData>;
		using MediaPinPtr = boost::shared_ptr<MediaPin>;
		using MediaPinRef = boost::weak_ptr<MediaPin>;
		using MediaPinGroup = UnorderedMap<const std::string, MediaPinPtr>;
		using MediaModulePtr = boost::shared_ptr<MediaModule>;

		class MediaFilter : public boost::enable_shared_from_this<MediaFilter>
		{
		public:
			MediaFilter(void);
			virtual ~MediaFilter(void);

		public:
			virtual int createNewFilter(void) = 0;
			virtual int destroyFilter(void);
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
			virtual int createNewModule(MediaDataPtr mediaData) = 0;
			int createNewInputPin(const std::string& pinID);
			int createNewOutputPin(const std::string& pinID);
			void setPostInputMediaDataCallback(void);
			int postInputMediaDataCallback(MediaDataPtr mediaData);

		protected:
			MediaPinGroup mediaPinGroup;
			MediaModulePtr mediaModulePtr;
		};//class MediaFilter
	}//namespace multimedia
}//namespace framework

#endif//MEDIA_FILTER_H
