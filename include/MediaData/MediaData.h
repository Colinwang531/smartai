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

#include "predef.h"

typedef enum class tagMediaDataMainID_t
{
	MEDIA_DATA_MAIN_ID_NONE = 0,
	MEDIA_DATA_MAIN_ID_VIDEO,
	MEDIA_DATA_MAIN_ID_AUDIO,
	MEDIA_DATA_MAIN_ID_IMAGE
}MediaDataMainID;

typedef enum class tagMediaDataSubID_t
{
	MEDIA_DATA_SUB_ID_NONE = 0,
	MEDIA_DATA_SUB_ID_HIKVISION,
	MEDIA_DATA_SUB_ID_H264,
	MEDIA_DATA_SUB_ID_H265,
	MEDIA_DATA_SUB_ID_YV12,
	MEDIA_DATA_SUB_ID_NV12,
	MEDIA_DATA_SUB_ID_BGR24,
	MEDIA_DATA_SUB_ID_YUV420P,
	MEDIA_DATA_SUB_ID_AAC,
	MEDIA_DATA_SUB_ID_G722,
	MEDIA_DATA_SUB_ID_JPEG,
	MEDIA_DATA_SUB_ID_BMP
}MediaDataSubID;

// Patch ID specifies the type of video frame in detail.
typedef enum class tagMediaDataPatchID_t
{
	MEDIA_DATA_PATCH_ID_NONE = 0,
	MEDIA_DATA_PATCH_ID_SPS,
	MEDIA_DATA_PATCH_ID_PPS,
	MEDIA_DATA_PATCH_ID_I,
	MEDIA_DATA_PATCH_ID_B,
	MEDIA_DATA_PATCH_ID_P,
}MediaDataPatchID;

namespace framework
{
	namespace multimedia
	{
		class MediaData
		{
		public:
			MediaData(
				const MediaDataMainID mainID = MediaDataMainID::MEDIA_DATA_MAIN_ID_NONE,
				const MediaDataSubID subID = MediaDataSubID::MEDIA_DATA_SUB_ID_NONE,
				const MediaDataPatchID pacthID = MediaDataPatchID::MEDIA_DATA_PATCH_ID_NONE);
			~MediaData(void);

		public:
			int setData(
				const unsigned char* data = NULL, const unsigned long long dataBytes = 0, const bool persist = false);
			inline void setUserData(void* data = NULL)
			{
				userData = data;
			}
			inline void* getUserData(void) const
			{
				return userData;
			}
			inline void setImageParameter(const int width = 0, const int height = 0)
			{
				imageWidth = width;
				imageHeight = height;
			}
			inline void getImageParameter(int& width, int& height) const
			{
				width = imageWidth;
				height = imageHeight;
			}
			inline const unsigned char* getData(void) const
			{
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
			int imageHeight;
			int imageWidth;
			unsigned char* mediaData;
			unsigned long long mediaDataBytes;
			void* userData;
			bool persistence;
		};//class MediaData
	}//namespace multimedia
}//namespace framework

#endif//MEDIA_DATA_H
