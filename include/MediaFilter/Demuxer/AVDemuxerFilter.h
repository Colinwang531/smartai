// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Video and audio demuxer filter class.
//

#ifndef AV_DEMUXER_FILTER_H
#define AV_DEMUXER_FILTER_H

#include "MediaFilter/SourceMediaFilter.h"

namespace framework
{
	namespace multimedia
	{
		class AVDemuxerFilter : public SourceMediaFilter
		{
		public:
			AVDemuxerFilter(void);
			virtual ~AVDemuxerFilter(void);

		protected:
			int createNewFilter(
				const MediaStreamID mediaStreamID = MediaStreamID::MEDIA_STREAM_ID_AV) override;
		};//class AVDemuxerFilter
	}//namespace multimedia
}//namespace framework

#endif//AV_DEMUXER_FILTER_H
