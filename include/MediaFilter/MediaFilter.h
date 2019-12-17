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

static const std::string AVCaptureFilterID = "AVCaptureFilterID";
static const std::string AVParserFilterID = "AVParserFilterID";
static const std::string AVControllerFilterID = "AVControllerFilterID";
static const std::string VideoDecoderFilterID = "VideoDecoderFilterID";
static const std::string AudioDecoderFilterID = "AudioDecoderFilterID";
static const std::string ImageConverterFilterID = "ImageConverterFilterID";
static const std::string VideoRendererFilterID = "VideoRendererFilterID";
static const std::string SoundPlayerFilterID = "SoundPlayerFilterID";
static const std::string AVCallbackFilterID = "AVCallbackFilterID";

namespace framework
{
	namespace multimedia
	{
// 		class MediaData;
// 		class MediaModule;
		class MediaPin;
// 		using MediaDataPtr = boost::shared_ptr<MediaData>;
 		using MediaPinPtr = boost::shared_ptr<MediaPin>;
// 		using MediaPinRef = boost::weak_ptr<MediaPin>;
		using MediaPinGroup = UnorderedMap<const std::string, MediaPinPtr>;
//		using MediaModulePtr = boost::shared_ptr<MediaModule>;

		class MediaFilter : public boost::enable_shared_from_this<MediaFilter>
		{
		public:
			MediaFilter(void);
			virtual ~MediaFilter(void);

		public:
			virtual int createNewFilter(const std::string filterID);
			virtual int destroyFilter(void);
//			virtual int inputMediaData(MediaDataPtr mediaData);
			inline MediaPinPtr queryMediaPinByID(const std::string pinID)
			{
				return mediaPinGroup.at(pinID);
			}

		protected:
			virtual bool isSourceFilter(void) const
			{
				return false;
			}
			virtual bool isTargetFilter(void) const
			{
				return false;
			}
// 			virtual int createNewModule(MediaDataPtr mediaData) = 0;
// 			void setPostInputMediaDataCallback(void);
// 			int postInputMediaDataCallback(MediaDataPtr mediaData);

		private:
			int createNewInputPin(const std::string pinID);
			int createNewOutputPin(const std::string pinID);

		protected:
			MediaPinGroup mediaPinGroup;
//			MediaModulePtr mediaModulePtr;
		};//class MediaFilter
	}//namespace multimedia
}//namespace framework

#endif//MEDIA_FILTER_H
