// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Base class of media data.
//

#ifndef MEDIA_DATA_H
#define MEDIA_DATA_H

#include "boost/shared_ptr.hpp"
#include "boost/weak_ptr.hpp"
#include "predef.h"

NS_BEGIN(media, 1)

typedef enum class tagMediaDataType_t
{
	MEDIA_DATA_TYPE_NONE = 0,
	MEDIA_DATA_TYPE_FILE_PATH,
	MEDIA_DATA_TYPE_FRAME_HEAD_INFO,
	MEDIA_DATA_TYPE_FRAME_DATA
}MediaDataType;

class MediaData
{
public:
	MediaData(const MediaDataType& type = MediaDataType::MEDIA_DATA_TYPE_NONE);
	~MediaData(void);

public:
	void resetMediaDataType(const MediaDataType& type = MediaDataType::MEDIA_DATA_TYPE_NONE);
	void resetMediaDataBuffer(void);
	int setMediaDataBuffer(const char* dataBuffer = NULL, const unsigned long long dataBytes = 0);
	inline boost::weak_ptr<char[]> getMediaDataBuffer(void) const
	{
		return mediaDataPtr;
	}
	inline const unsigned long long getMediaDataBytes(void) const
	{
		return mediaDataBytes;
	}
	inline const MediaDataType& getMediaDataType(void) const
	{
		return mediaDataType;
	}

private:
	MediaDataType mediaDataType;
	boost::shared_ptr<char[]> mediaDataPtr;
	unsigned long long mediaDataBytes;
};//class MediaData

NS_END

#endif//MEDIA_DATA_H
