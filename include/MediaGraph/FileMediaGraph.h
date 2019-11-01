// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Base class of file graph.
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

public:
	int startMediaGraph(const std::string filePath) override;
	int stopMediaGraph(void) override;
	virtual int getTotalTime(
		long long& hour, long long& minute, long long& second) = 0;
	virtual int getBitrate(long long& bitrate) = 0;

protected:
	virtual int openFile(const std::string filePath) = 0;
	virtual int closeFile(void) = 0;
	virtual int buildGraph(void) = 0;
	int startPlay(void) override;
	int stopPlay(void) override;
	int pausePlay(void) override;
	int fastPlay(const short speed = 1) override;
	int slowPlay(const short speed = -1) override;

private:
	int checkSpeedValue(const short speed = 0);
};//class FileMediaGraph

NS_END

#endif//FILE_MEDIA_GRAPH_H
