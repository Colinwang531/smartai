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

#include "boost/enable_shared_from_this.hpp"
#include "boost/shared_ptr.hpp"
#include "boost/thread/mutex.hpp"
#include "boost/unordered_map.hpp"
#include "MediaPin/MediaPin.h"
using MediaPinPtr = boost::shared_ptr<NS(pin, 1)::MediaPin>;
using MediaPinGroup = boost::unordered_map<const std::string, MediaPinPtr>;
#include "MediaData/MediaData.h"
using MediaDataPtr = boost::shared_ptr<NS(media, 1)::MediaData>;

NS_BEGIN(filter, 1)

class MediaFilter : protected boost::enable_shared_from_this<MediaFilter>
{
public:
	MediaFilter(void);
	virtual ~MediaFilter(void);

public:
	// Media pin(s) instance create and destroy by filter object itself, 
	// caller use this method to query instance of pin only. 
	virtual MediaPinPtr queryPin(const std::string pinID);
	virtual int inputData(MediaDataPtr dataPtr);
	virtual bool isSourceFilter(void) const;
	virtual bool isTargetFilter(void) const;

protected:
	boost::mutex mtx;
	MediaPinGroup mediaPinGroup;
};//class MediaFilter

NS_END

#endif//MEDIA_FILTER_H
