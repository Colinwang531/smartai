// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Live and playback stream media data base class of graph.
//

#ifndef STREAM_MEDIA_GRAPH_H
#define STREAM_MEDIA_GRAPH_H

#include "MediaGraph.h"

NS_BEGIN(graph, 1)

class StreamMediaGraph : public MediaGraph
{
public:
	StreamMediaGraph(void);
	virtual ~StreamMediaGraph(void);

protected:
	virtual int openStream(const std::string streamUrl) = 0;
	virtual int closeStream(void) = 0;
};//class StreamMediaGraph

NS_END

#endif//STREAM_MEDIA_GRAPH_H
