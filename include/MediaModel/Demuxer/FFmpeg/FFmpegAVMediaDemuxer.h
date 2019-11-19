// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// AV media demuxer using FFMPEG library.
//

#ifndef FFMPEG_AV_MEDIA_DEMUXER_H
#define FFMPEG_AV_MEDIA_DEMUXER_H

extern "C"
{
#include "libavformat/avformat.h"
}
#include "MediaModel/Demuxer/MediaDemuxer.h"

NS_BEGIN(model, 1)

class FFmpegAVMediaDemuxer : public MediaDemuxer
{
public:
	FFmpegAVMediaDemuxer(void);
	virtual ~FFmpegAVMediaDemuxer(void);

public:
	int openStream(const std::string streamUrl) override;
	int closeStream(void) override;
	const long long getTotalSeconds(void) const;
	const long long getBitrate(void) const;
	const MediaDataSubID getVideoStreamID(void) const override;
	const MediaDataSubID getAudioStreamID(void) const override;

protected:
	int inputMediaData(MediaDataPtr mediaData) override;
	void pullStreamDataProcess(void) override;

private:
	AVFormatContext* formatctx;
	AVPacket packet;
	int videoStreamIndex;
	int audioStreamIndex;
};//class FFmpegAVMediaDemuxer

NS_END

#endif//FFMPEG_AV_MEDIA_DEMUXER_H
