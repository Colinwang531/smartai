// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Live stream media data base class of graph.
//

#ifndef LIVE_STREAM_MEDIA_GRAPH_H
#define LIVE_STREAM_MEDIA_GRAPH_H

#include "MediaGraph/StreamMediaGraph.h"

NS_BEGIN(graph, 1)

class LivestreamMediaGraph : public StreamMediaGraph
{
public:
	LivestreamMediaGraph(void);
	virtual ~LivestreamMediaGraph(void);
};//class LivestreamMediaGraph

NS_END

#endif//LIVE_STREAM_MEDIA_GRAPH_H
