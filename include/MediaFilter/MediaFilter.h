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
#include "MediaPin/MediaPin.h"
using MediaPinPtr = boost::shared_ptr<NS(pin, 1)::MediaPin>;
#include "MediaModel/MediaModel.h"
using MediaModelPtr = boost::shared_ptr<NS(model, 1)::MediaModel>;
using MediaModelRef = boost::weak_ptr<NS(model, 1)::MediaModel>;
#include "DataStruct/UnorderedMap.h"
using MediaPinGroup = UnorderedMap<const std::string, MediaPinPtr>;

NS_BEGIN(filter, 1)

BOOST_STATIC_CONSTANT(std::string, AVMediaDemuxerFilterID = "AVMediaDemuxerFilterID");
BOOST_STATIC_CONSTANT(std::string, AVMediaLivestreamCaptureFilterID = "AVMediaLivestreamCaptureFilterID");
BOOST_STATIC_CONSTANT(std::string, AVMediaControllerFilterID = "AVMediaControllerFilterID");
BOOST_STATIC_CONSTANT(std::string, AVMediaVideoDecoderFilterID = "AVMediaVideoDecoderFilterID");
BOOST_STATIC_CONSTANT(std::string, AVMediaAudioDecoderFilterID = "AVMediaAudioDecoderFilterID");
BOOST_STATIC_CONSTANT(std::string, AVMediaVideoRendererFilterID = "AVMediaVideoRendererFilterID");
BOOST_STATIC_CONSTANT(std::string, AVMediaAudioRendererFilterID = "AVMediaAudioRendererFilterID");
BOOST_STATIC_CONSTANT(std::string, AVMediaVideoFormatterFilterID = "AVMediaVideoFormatterFilterID");
BOOST_STATIC_CONSTANT(std::string, AVMediaDataCaptureFilterID = "AVMediaDataCaptureFilterID");

class MediaFilter 
	: public boost::enable_shared_from_this<MediaFilter>
{
public:
	MediaFilter(void);
	virtual ~MediaFilter(void);

public: 
	virtual int createNewFilter(void);
	virtual int destroyFilter(void) = 0;
	inline MediaPinRef queryMediaPinByID(const std::string pinID)
	{
		return mediaPinGroup.at(pinID);
	}
	inline MediaModelRef getMediaModel(void) const
	{
		return mediaModelPtr;
	}
	// Receive media data package from pin instance.
	virtual int inputMediaData(MediaDataPtr mediaData) = 0;
	virtual bool isSourceFilter(void) const
	{
		return false;
	}
	virtual bool isTargetFilter(void) const
	{
		return false;
	}

protected:
	int createNewInputPin(const std::string pinID);
	int createNewOutputPin(const std::string pinID);
	int postInputMediaData(MediaDataPtr mediaData);

protected:
	MediaPinGroup mediaPinGroup;
	MediaModelPtr mediaModelPtr;
};//class MediaFilter

NS_END

#endif//MEDIA_FILTER_H
