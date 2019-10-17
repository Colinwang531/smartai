// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Playback media data base class of graph.
//

#ifndef PLAYBACK_MEDIA_GRAPH_H
#define PLAYBACK_MEDIA_GRAPH_H

#include "MediaGraph/StreamMediaGraph.h"

NS_BEGIN(graph, 1)

class PlaybackMediaGraph : public StreamMediaGraph
{
public:
	PlaybackMediaGraph(void);
	virtual ~PlaybackMediaGraph(void);

protected:
	virtual int pausePlay(void) = 0;
	virtual int FastPlay(const unsigned short speed = 0) = 0;
	virtual int slowPlay(const unsigned short speed = 0) = 0;
};//class PlaybackMediaGraph

NS_END

#endif//PLAYBACK_MEDIA_GRAPH_H
