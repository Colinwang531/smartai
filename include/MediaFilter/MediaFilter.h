//
//		Copyright :					@2019, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2019-10-13
//		Description:					多媒体过滤器抽象基类
//
//		History:						Author									Date														Description
//											王科威									2019-10-13										创建
//

#ifndef MEDIA_FILTER_H
#define MEDIA_FILTER_H

#include "boost/shared_ptr.hpp"
#include "MediaPin/MediaPin.h"
using MediaPinPtr = boost::shared_ptr<NS(pin, 1)::MediaPin>;
#include "DataStruct/UnorderedMap.h"
using MediaPinGroup = NS(datastruct, 1)::UnorderedMap<const std::string, MediaPinPtr>;
#include "MediaData/MediaData.h"
using MediaDataPtr = boost::shared_ptr<NS(media, 1)::MediaData>;

NS_BEGIN(filter, 1)

class MediaFilter
{
public:
	MediaFilter(void);
	virtual ~MediaFilter(void);

public:
	virtual int addPin(
		const std::string pinID, MediaPinPtr pinPtr);
	virtual int removePin(const std::string pinID);
	virtual int inputData(MediaDataPtr dataPtr) = 0;
	virtual bool isSourceFilter(void) const;
	virtual bool isTargetFilter(void) const;

protected:
	MediaPinGroup mediaPinGroup;
};//class MediaFilter

NS_END

#endif//MEDIA_FILTER_H
