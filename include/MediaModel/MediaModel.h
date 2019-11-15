// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Abstract base class of filter model.
//

#ifndef MEDIA_MODEL_H
#define MEDIA_MODEL_H

#include "boost/shared_ptr.hpp"
#include "boost/function.hpp"
#include "MediaData/MediaData.h"
using MediaDataPtr = boost::shared_ptr<NS(media, 1)::MediaData>;

NS_BEGIN(model, 1)

typedef boost::function<int(const MediaDataSubID, const MediaDataSubID)> NotifiyMediaDataSubIDCallback;
typedef boost::function<int(MediaDataPtr)> PostMediaDataCallback;

class MediaModel
{
public:
	MediaModel(void);
	virtual ~MediaModel(void);

public:
	inline void setPostMediaDataCallback(PostMediaDataCallback callback = NULL)
	{
		postMediaDataCallback = callback;
	}
	inline void setNotifyMediaStreamIDCallback(NotifiyMediaDataSubIDCallback callback = NULL)
	{
		notifyMediaDataSubIDCallback = callback;
	}
	virtual int inputMediaData(MediaDataPtr mediaData) = 0;

protected:
	PostMediaDataCallback postMediaDataCallback;
	NotifiyMediaDataSubIDCallback notifyMediaDataSubIDCallback;
};//class MediaModel

NS_END

#endif//MEDIA_MODEL_H
