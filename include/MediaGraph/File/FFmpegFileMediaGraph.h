// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Base class of local file graph using FFMPEG library.
//

#ifndef FFMPEG_FILE_MEDIA_GRAPH_H
#define FFMPEG_FILE_MEDIA_GRAPH_H

#include "MediaGraph/FileMediaGraph.h"
#include "MediaDemuxer/MediaDemuxer.h"
using MediaStreamType = NS(demuxer, 1)::MediaStreamType;

NS_BEGIN(graph, 1)

class FFmpegFileMediaGraph : public FileMediaGraph
{
public:
	FFmpegFileMediaGraph(void);
	virtual ~FFmpegFileMediaGraph(void);

protected:
	int inputMediaData(MediaDataPtr mediaData) override;
	const long long getTotalSeconds(void) override;
	const long long getBitrate(void) override;

private:
	int createNewDemuxerFilter(MediaDataPtr mediaData);
	int createNewVideoDecoderFilter(
		const MediaStreamType videoType = MediaStreamType::MEDIA_STREAM_TYPE_NONE);
	int createNewAudioDecoderFilter(
		const MediaStreamType audioType = MediaStreamType::MEDIA_STREAM_TYPE_NONE);
};//class FFmpegFileMediaGraph

NS_END

#endif//FFMPEG_FILE_MEDIA_GRAPH_H
