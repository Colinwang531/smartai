// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Base class of graph for live stream.
//

#ifndef LIVE_STREAM_MEDIA_GRAPH_H
#define LIVE_STREAM_MEDIA_GRAPH_H

#include "MediaGraph.h"

NS_BEGIN(graph, 1)

class LiveStreamMediaGraph : public MediaGraph
{
public:
	LiveStreamMediaGraph(void);
	virtual ~LiveStreamMediaGraph(void);
};//class LiveStreamMediaGraph

NS_END

#endif//LIVE_STREAM_MEDIA_GRAPH_H
