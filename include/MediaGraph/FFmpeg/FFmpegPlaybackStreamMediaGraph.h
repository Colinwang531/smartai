// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Class of graph for playing file stream using FFMPEG library.
//

#ifndef FFMPEG_PLAYBACK_STREAM_MEDIA_GRAPH_H
#define FFMPEG_PLAYBACK_STREAM_MEDIA_GRAPH_H

#include "MediaGraph/PlaybackStreamMediaGraph.h"

NS_BEGIN(graph, 1)

class FFmpegPlaybackStreamMediaGraph : public PlaybackStreamMediaGraph
{
public:
	FFmpegPlaybackStreamMediaGraph(void);
	virtual ~FFmpegPlaybackStreamMediaGraph(void);

protected:
	int openMediaGraph(const std::string streamUrl, void* hwnd = NULL) override;
	int closeMediaGraph(void) override;

private:
	int createNewMediaDemuxer(const std::string streamUrl);
	int createNewMediaController(void);
	int runPullStreamDataThread(void);
};//class FFmpegPlaybackStreamMediaGraph

NS_END

#endif//FFMPEG_PLAYBACK_STREAM_MEDIA_GRAPH_H
