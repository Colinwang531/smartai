// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Base class of target filter.
//

#ifndef TARGET_MEDIA_FILTER_H
#define TARGET_MEDIA_FILTER_H

#include "MediaFilter.h"

//typedef void (CALLBACK* PostMediaDataCallback)(const int streamID, const unsigned char* mediaData, const int dataBytes, void* userData);

namespace framework
{
	namespace multimedia
	{
		class TargetMediaFilter : public MediaFilter
		{
		public:
			TargetMediaFilter(void);
			virtual ~TargetMediaFilter(void);

// 		public:
// 			inline void setMediaDataCallback(const int streamID = 0, PostMediaDataCallback callback = NULL, void* userData = NULL)
// 			{
// 				mediaStreamID = streamID;
// 				postMediaDataCallback = callback;
// 				cbUserData = userData;
// 			}

		protected:
			int inputMediaData(MediaDataPtr mediaData) override;
			bool isTargetFilter(void) const override
			{
				return true;
			}

// 		protected:
// 			int mediaStreamID;
// 			PostMediaDataCallback postMediaDataCallback;
// 			void* cbUserData;
		};//class TargetMediaFilter
	}//namespace multimedia
}//namespace framework

#endif//TARGET_MEDIA_FILTER_H
