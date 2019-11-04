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

extern "C"
{
#include "libavformat/avformat.h"
}
#include "MediaDemuxer/MediaDemuxer.h"

NS_BEGIN(demuxer, 1)

class FFmpegFileMediaDemuxer : public MediaDemuxer
{
public:
	FFmpegFileMediaDemuxer(void);
	virtual ~FFmpegFileMediaDemuxer(void);

protected:
	int inputMediaData(MediaDataPtr mediaData) override;

private:
	AVFormatContext* avFormatContext;
};//class FFmpegFileMediaDemuxer

NS_END

#endif//FFMPEG_FILE_MEDIA_DEMUXER_H
