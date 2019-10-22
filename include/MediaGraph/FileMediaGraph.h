// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Local and network file media data base class of graph.
//

#ifndef FILE_MEDIA_GRAPH_H
#define FILE_MEDIA_GRAPH_H

#include "MediaGraph/File/MediaPlayController.h"
#include "MediaGraph.h"

NS_BEGIN(graph, 1)

class FileMediaGraph 
	: public MediaGraph, protected EnableMediaPlayControl
{
public:
	FileMediaGraph(void);
	virtual ~FileMediaGraph(void);

protected:
	virtual int openFile(const char* filePath = NULL);
	virtual int closeFile(void) = 0;
	int startPlay(void) override;
	int stopPlay(void) override;
	int pausePlay(void) override;
	int fastPlay(const short speed = 1) override;
	int slowPlay(const short speed = 1) override;

private:
	int checkSpeedValue(const short speed = 1);
};//class FileMediaGraph

NS_END

#endif//FILE_MEDIA_GRAPH_H
