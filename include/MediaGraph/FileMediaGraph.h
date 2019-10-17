// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : ������
// E-mail : wangkw531@icloud.com
//
// Local and network file media data base class of graph.
//

#ifndef FILE_MEDIA_GRAPH_H
#define FILE_MEDIA_GRAPH_H

#include "MediaGraph/File/EnableMediaPlayControl.h"
#include "MediaGraph.h"

NS_BEGIN(graph, 1)

class FileMediaGraph 
	: public MediaGraph, protected EnableMediaPlayControl
{
public:
	FileMediaGraph(void);
	virtual ~FileMediaGraph(void);

protected:
	virtual int openFile(void) = 0;
	virtual int closeFile(void) = 0;
	int play(void) override;
	int stop(void) override;
	int pause(void) override;
	int fast(const unsigned short speed = 1) override;
	int slow(const unsigned short speed = 1) override;

private:
	int checkSpeedValue(const unsigned short speed = 1);
};//class FileMediaGraph

NS_END

#endif//FILE_MEDIA_GRAPH_H
