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

#include "MediaPin/MediaPin.h"
using MediaPinPtr = boost::shared_ptr<NS(pin, 1)::MediaPin>;
using MediaPinRef = boost::weak_ptr<NS(pin, 1)::MediaPin>;
#include "DataStruct/UnorderedMap.h"
using MediaPinGroup = UnorderedMap<const std::string, MediaPinPtr>;

NS_BEGIN(filter, 1)

typedef enum class tagMediaFilterMode_t
{
	MEDIA_FILTER_MODE_SOURCE = 0,
	MEDIA_FILTER_MODE_MEDIUM,
	MEDIA_FILTER_MODE_TARGET
}MediaFilterMode;

BOOST_STATIC_CONSTANT(std::string, MediaDemuxerFilterID = "MediaDemuxerFilterID");

class MediaFilter
{
public:
	MediaFilter(const MediaFilterMode mode = MediaFilterMode::MEDIA_FILTER_MODE_MEDIUM);
	virtual ~MediaFilter(void);

public: 
	virtual MediaPinRef queryMediaPinByID(const std::string pinID);
	virtual int inputMediaData(MediaDataPtr mediaData) = 0;
	inline const MediaFilterMode getMediaFilterMode(void) const
	{
		return mediaFilterMode;
	}

protected:
	MediaPinGroup mediaPinGroup;

private:
	const MediaFilterMode mediaFilterMode;
};//class MediaFilter

NS_END

#endif//MEDIA_FILTER_H
