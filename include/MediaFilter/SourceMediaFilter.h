// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Base class of source filter.
//

#ifndef SOURCE_MEDIA_FILTER_H
#define SOURCE_MEDIA_FILTER_H

#include "MediaFilter.h"

namespace framework
{
	namespace multimedia
	{
		class SourceMediaFilter : public MediaFilter
		{
		public:
			SourceMediaFilter(void);
			virtual ~SourceMediaFilter(void);

		protected:
			bool isSourceFilter(void) const override
			{
				return true;
			}
			int inputMediaData(MediaDataPtr mediaData) override;
		};//class SourceMediaFilter
	}//namespace multimedia
}//namespace framework

#endif//SOURCE_MEDIA_FILTER_H
