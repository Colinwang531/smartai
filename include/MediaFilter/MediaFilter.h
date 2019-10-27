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
using MediaPinRef = boost::weak_ptr<NS(pin, 1)::MediaPin>;
#include "DataStruct/UnorderedMap.h"
using MediaPinGroup = UnorderedMap<const std::string, MediaPinPtr>;

NS_BEGIN(filter, 1)

typedef enum class tagMediaFilterMode_t
{
	MEDIA_FILTER_SOURCE = 0,
	MEDIA_FILTER_MEDIUM,
	MEDIA_FILTER_TARGET
}MediaFilterMode;

class MediaFilter : protected boost::enable_shared_from_this<MediaFilter>
{
public:
	MediaFilter(const MediaFilterMode mode = MediaFilterMode::MEDIA_FILTER_MEDIUM);
	virtual ~MediaFilter(void);

public: 
	virtual const MediaPinRef& queryPinByID(const std::string pinID);
	virtual int addPin(const std::string pinID, MediaPinPtr pinPtr);
	virtual int removePin(const std::string pinID);
	virtual int inputData(MediaDataPtr dataPtr);
	inline const MediaFilterMode getMode(void) const
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
