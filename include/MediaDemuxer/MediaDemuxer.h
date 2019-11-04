// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Base class of video and audio demuxer .
//

#ifndef MEDIA_DEMUXER_H
#define MEDIA_DEMUXER_H

#include "MediaData/MediaData.h"
using MediaDataPtr = boost::shared_ptr<NS(media, 1)::MediaData>;

NS_BEGIN(demuxer, 1)

class MediaDemuxer
{
public:
	MediaDemuxer(void);
	virtual ~MediaDemuxer(void);

public:
	virtual int inputMediaData(MediaDataPtr mediaData) = 0;
};//class MediaDemuxer

NS_END

#endif//MEDIA_DEMUXER_H
