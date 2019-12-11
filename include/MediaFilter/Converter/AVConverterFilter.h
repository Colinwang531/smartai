// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Base class of filter for data converting.
//

#ifndef AV_CONVERTER_FILTER_H
#define AV_CONVERTER_FILTER_H

#include "MediaFilter/MediaFilter.h"

namespace framework
{
	namespace multimedia
	{
		class AVConverterFilter : public MediaFilter
		{
		public:
			AVConverterFilter(void);
			virtual ~AVConverterFilter(void);

		protected:
			int createNewFilter(void) override;
			int createNewModule(MediaDataPtr mediaData) override;
		};//class AVConverterFilter
	}//namespace multimedia
}//namespace framework

#endif//AV_CONVERTER_FILTER_H
