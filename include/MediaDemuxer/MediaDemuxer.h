// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Base class of video and audio demuxer.
//

#ifndef MEDIA_DEMUXER_H
#define MEDIA_DEMUXER_H

#include "MediaData/MediaData.h"
using MediaDataPtr = boost::shared_ptr<NS(media, 1)::MediaData>;

NS_BEGIN(demuxer, 1)

typedef enum class tagMediaStreamType_t
{
	MEDIA_STREAM_TYPE_NONE = 0,
	MEDIA_STREAM_TYPE_H264,
	MEDIA_STREAM_TYPE_H265,
	MEDIA_STREAM_TYPE_AAC,
	MEDIA_STREAM_TYPE_G722
}MediaStreamType;

class MediaDemuxer
{
public:
	MediaDemuxer(void);
	virtual ~MediaDemuxer(void);

public:
	virtual int inputMediaData(MediaDataPtr mediaData) = 0;
	//Suppose only one type of video/audio stream in media file.
	virtual const MediaStreamType getVideoStreamType(void) const;
	virtual const MediaStreamType getAudioStreamType(void) const;
};//class MediaDemuxer

NS_END

#endif//MEDIA_DEMUXER_H
