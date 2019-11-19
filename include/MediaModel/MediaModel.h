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

#include "boost/function.hpp"
#include "MediaData/MediaData.h"
using MediaDataPtr = boost::shared_ptr<NS(media, 1)::MediaData>;

NS_BEGIN(model, 1)

typedef boost::function<int(MediaDataPtr)> PostInputMediaDataCallback;

class MediaModel
{
public:
	MediaModel(void);
	virtual ~MediaModel(void);

public:
	inline void setPostInputMediaDataCallback(PostInputMediaDataCallback callback = NULL)
	{
		postInputMediaDataCallback = callback;
	}
	virtual int inputMediaData(MediaDataPtr mediaData) = 0;

protected:
	PostInputMediaDataCallback postInputMediaDataCallback;
};//class MediaModel

NS_END

#endif//MEDIA_MODEL_H
