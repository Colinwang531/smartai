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

#include "MediaDemuxer/MediaDemuxer.h"

NS_BEGIN(demuxer, 1)

class FFmpegAVMediaDemuxer : public MediaDemuxer
{
public:
	FFmpegAVMediaDemuxer(void);
	virtual ~FFmpegAVMediaDemuxer(void);

public:
	const long long getTotalSeconds(void) const;
	const long long getBitrate(void) const;
	int openStream(const std::string streamUrl) override;
	int closeStream(void) override;
	const MediaStreamID getVideoStreamID(void) const override;
	const MediaStreamID getAudioStreamID(void) const override;

protected:
	void pullStreamDataProcess(void) override;
	void getVideoAndAudioMediaIndex(void);

private:
	void* avFormatContext;
	void* avPacket;
	int videoMediaIndex;
	int audioMediaIndex;
};//class FFmpegAVMediaDemuxer

NS_END

#endif//FFMPEG_AV_MEDIA_DEMUXER_H
