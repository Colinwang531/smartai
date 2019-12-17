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

//#include "boost/function.hpp"
#include "MediaFilter.h"

namespace framework
{
	namespace multimedia
	{
//		typedef boost::function<void(const int, MediaDataPtr)> CallbackMediaDataCallback;

		class TargetMediaFilter : public MediaFilter
		{
		public:
			TargetMediaFilter(void);
			virtual ~TargetMediaFilter(void);

		public:
// 			inline void setMediaDataCallback(
// 				const int id = 0, CallbackMediaDataCallback callback = NULL)
// 			{
// 				userID = id;
// 				callbackMediaDataCallback = callback;
// 			}

		protected:
//			int inputMediaData(MediaDataPtr mediaData) override;
			bool isTargetFilter(void) const override
			{
				return true;
			}

		protected:
			// The identity according to the definition by the caller.
			int userID;
//			CallbackMediaDataCallback callbackMediaDataCallback;
		};//class TargetMediaFilter
	}//namespace multimedia
}//namespace framework

#endif//TARGET_MEDIA_FILTER_H
