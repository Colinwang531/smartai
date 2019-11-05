// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// File media demuxer using FFMPEG library.
//

#ifndef FFMPEG_FILE_MEDIA_DEMUXER_H
#define FFMPEG_FILE_MEDIA_DEMUXER_H

#include "MediaDemuxer/MediaDemuxer.h"

NS_BEGIN(demuxer, 1)

class FFmpegFileMediaDemuxer : public MediaDemuxer
{
public:
	FFmpegFileMediaDemuxer(void);
	virtual ~FFmpegFileMediaDemuxer(void);

public:
	const long long getTotalSeconds(void) const;
	const long long getBitrate(void) const;

protected:
	int inputMediaData(MediaDataPtr mediaData) override;
	const MediaStreamType getVideoStreamType(void) const override;
	const MediaStreamType getAudioStreamType(void) const override;

private:
	void* avFormatContext;
};//class FFmpegFileMediaDemuxer

NS_END

#endif//FFMPEG_FILE_MEDIA_DEMUXER_H
