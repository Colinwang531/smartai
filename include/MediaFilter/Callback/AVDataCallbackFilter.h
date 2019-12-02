// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Video and audio data callback.
//

#ifndef AV_DATA_CALLBACK_FILTER_H
#define AV_DATA_CALLBACK_FILTER_H

#include "MediaFilter/TargetMediaFilter.h"

namespace framework
{
	namespace multimedia
	{
		class AVDataCallbackFilter : public TargetMediaFilter
		{
		public:
			AVDataCallbackFilter(void);
			virtual ~AVDataCallbackFilter(void);
		};//class AVDataCallbackFilter
	}//namespace multimedia
}//namespace framework

#endif//AV_DATA_CALLBACK_FILTER_H
