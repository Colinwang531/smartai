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

#include "boost/noncopyable.hpp"
#include "DataStruct/UnorderedMap.h"

static const std::string AVMediaDemuxerFilterID = "AVMediaDemuxerFilterID";
static const std::string AVMediaLivestreamCaptureFilterID = "AVMediaLivestreamCaptureFilterID";
static const std::string AVMediaPlaybackControlFilterID = "AVMediaPlaybackControlFilterID";
static const std::string AVMediaVideoDecoderFilterID = "AVMediaVideoDecoderFilterID";
static const std::string AVMediaAudioDecoderFilterID = "AVMediaAudioDecoderFilterID";
static const std::string AVMediaVideoRendererFilterID = "AVMediaVideoRendererFilterID";
static const std::string AVMediaSoundPlayerFilterID = "AVMediaSoundPlayerFilterID";
static const std::string AVMediaImageFormatFilterID = "AVMediaImageFormatFilterID";
static const std::string AVMediaDataCallbackFilterID = "AVMediaDataCallbackFilterID";

namespace framework
{
	namespace multimedia
	{
		class MediaModel;
		class MediaPin;
		using MediaPinGroup = UnorderedMap<const std::string, MediaPin*>;

		class MediaFilter : private boost::noncopyable
		{
		public:
			MediaFilter(void);
			virtual ~MediaFilter(void);

		public:
			virtual int createNewFilter(void);
			virtual int destroyFilter(void);
			virtual int inputMediaData(MediaData* mediaData = NULL);
			virtual bool isSourceFilter(void) const;
			virtual bool isTargetFilter(void) const;
			inline MediaPin* queryMediaPinByID(const std::string& pinID)
			{
				return mediaPinGroup.at(pinID);
			}
			inline MediaModel* getMediaModel(void) const
			{
				return mediaModel;
			}

		protected:
			int createNewInputPin(const std::string& pinID);
			int createNewOutputPin(const std::string& pinID);
			int postInputMediaData(MediaData* mediaData = NULL);

		protected:
			MediaPinGroup mediaPinGroup;
			MediaModel* mediaModel;
		};//class MediaFilter
	}//namespace multimedia
}//namespace framework

#endif//MEDIA_FILTER_H
