// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Base class of AV demuxer.
//

#ifndef MEDIA_DEMUXER_H
#define MEDIA_DEMUXER_H

#include "MediaModel/MediaModel.h"

namespace framework
{
	namespace multimedia
	{
		class MediaDemuxer : public MediaModel
		{
		public:
			MediaDemuxer(void);
			virtual ~MediaDemuxer(void);

		public:
			virtual int openStream(const std::string& streamUrl);
			virtual int closeStream(void);

		protected:
			int inputMediaData(MediaDataPtr mediaData) override;
			virtual void mediaDemuxerWorkerThread(void) = 0;

		protected:
			bool stopped;
		};//class MediaDemuxer
	}//namespace multimedia
}//namespace framework

#endif//MEDIA_DEMUXER_H
