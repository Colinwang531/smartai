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
#include "predef.h"

NS_BEGIN(media, 1)

class MediaData
{
public:
	MediaData(
		const MediaDataMainID mainID = MediaDataMainID::MEDIA_DATA_MAIN_ID_NONE,
		const MediaDataSubID subID = MediaDataSubID::MEDIA_DATA_SUB_ID_NONE,
		const MediaDataPatchID pacthID = MediaDataPatchID::MEDIA_DATA_PATCH_ID_NONE);
	~MediaData(void);

public:
	int copyData(const unsigned char* data = NULL, const unsigned long long dataBytes = 0);
	int setData(
		const unsigned char* data = NULL, const unsigned long long dataBytes = 0);
	inline void setRawData(void* data = NULL)
	{
		mediaDataRaw = data;
	}
	inline void* getRawData(void) const
	{
		return mediaDataRaw;
	}
	inline void setUserData(void* data = NULL)
	{
		userData = data;
	}
	inline void* getUserData(void) const
	{
		return userData;
	}
	inline void setPixel(const int width = 0, const int height = 0)
	{
		imageWidth = width;
		imageHeight = height;
	}
	inline const int getWidth(void) const 
	{
		return imageWidth;
	}
	inline const int getHeight(void) const
	{
		return imageHeight;
	}
	inline const unsigned char* getData(void) const
	{
//		return mediaDataPtr ? mediaDataPtr.get() : NULL;
		return mediaData;
	}
	inline const unsigned long long getDataBytes(void) const
	{
		return mediaDataBytes;
	}
	inline const MediaDataMainID getMainID(void) const
	{
		return mediaDataMainID;
	}
	inline const MediaDataSubID getSubID(void) const
	{
		return mediaDataSubID;
	}
	inline const MediaDataPatchID getPatchID(void) const
	{
		return mediaDataPatchID;
	}

private:
	const MediaDataMainID mediaDataMainID;
	const MediaDataSubID mediaDataSubID;
	const MediaDataPatchID mediaDataPatchID;
//	boost::shared_ptr<unsigned char[]> mediaDataPtr;
	unsigned char* mediaData;
	unsigned long long mediaDataBytes;
	int imageHeight;
	int imageWidth;
	void* mediaDataRaw;
	void* userData;
	bool copyMediaData;
};//class MediaData

NS_END

#endif//MEDIA_DATA_H
