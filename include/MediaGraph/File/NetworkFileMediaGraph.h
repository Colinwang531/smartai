// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Network file media data base class of graph.
//

#ifndef NETWORK_FILE_MEDIA_GRAPH_H
#define NETWORK_FILE_MEDIA_GRAPH_H

#include "MediaGraph/FileMediaGraph.h"

NS_BEGIN(graph, 1)

class NetworkFileMediaGraph : public FileMediaGraph
{
public:
	NetworkFileMediaGraph(void);
	virtual ~NetworkFileMediaGraph(void);
};//class NetworkFileMediaGraph

NS_END

#endif//NETWORK_FILE_MEDIA_GRAPH_H
