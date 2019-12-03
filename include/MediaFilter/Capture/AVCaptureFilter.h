// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Live stream capture.
//

#ifndef LIVE_STREAM_CAPTURE_FILTER_H
#define LIVE_STREAM_CAPTURE_FILTER_H

#include "MediaFilter/SourceMediaFilter.h"

namespace framework
{
	namespace multimedia
	{
		class AVCaptureFilter : public SourceMediaFilter
		{
		public:
			AVCaptureFilter(void);
			virtual ~AVCaptureFilter(void);

		protected:
			int createNewModel(MediaDataPtr mediaData) override;
		};//class AVCaptureFilter
	}//namespace multimedia
}//namespace framework

#endif//LIVE_STREAM_CAPTURE_FILTER_H
