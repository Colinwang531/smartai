// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Base class of graph for disposing local file.
//

#ifndef LOCAL_FILE_PLAYBACK_MEDIA_GRAPH_H
#define LOCAL_FILE_PLAYBACK_MEDIA_GRAPH_H

#include "MediaGraph/PlaybackMediaGraph.h"

NS_BEGIN(graph, 1)

class LocalFilePlaybackMediaGraph : public PlaybackMediaGraph
{
public:
	LocalFilePlaybackMediaGraph(void);
	virtual ~LocalFilePlaybackMediaGraph(void);
};//class FFmpegPlaybackStreamMediaGraph

NS_END

#endif//LOCAL_FILE_PLAYBACK_MEDIA_GRAPH_H
