// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Local file media data base class of graph.
//

#ifndef LOCAL_FILE_MEDIA_GRAPH_H
#define LOCAL_FILE_MEDIA_GRAPH_H

#include "MediaGraph/FileMediaGraph.h"

NS_BEGIN(graph, 1)

class LocalFileMediaGraph : public FileMediaGraph
{
public:
	LocalFileMediaGraph(void);
	virtual ~LocalFileMediaGraph(void);

protected:
	int openFile(const char* filePath = NULL) override;
	int closeFile(void) override;
};//class LocalFileMediaGraph

NS_END

#endif//LOCAL_FILE_MEDIA_GRAPH_H
