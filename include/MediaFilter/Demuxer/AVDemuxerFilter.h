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
#include "MediaDemuxer/MediaDemuxer.h"
using MediaDemuxerPtr = boost::shared_ptr<NS(demuxer, 1)::MediaDemuxer>;

NS_BEGIN(filter, 1)

class AVDemuxerFilter : public SourceMediaFilter
{
public:
	AVDemuxerFilter(void);
	virtual ~AVDemuxerFilter(void);

public:
	int createNewMediaDemuxer(const std::string streamUrl);
	int runPullStreamDataThread(void);
	const long long getTotalSeconds(void) const;
	const long long getBitrate(void) const;
	const MediaStreamID getVideoStreamID(void) const;
	const MediaStreamID getAudioStreamID(void) const;

private:
	int createNewOutputPin(void);

protected:
	MediaDemuxerPtr mediaDemuxerPtr;
};//class AVDemuxerFilter

NS_END

#endif//AV_DEMUXER_FILTER_H
