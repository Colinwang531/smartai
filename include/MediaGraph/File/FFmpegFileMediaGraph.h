// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Base class of local file graph using FFMPEG library.
//

#ifndef FFMPEG_FILE_MEDIA_GRAPH_H
#define FFMPEG_FILE_MEDIA_GRAPH_H

#include "MediaGraph/FileMediaGraph.h"

NS_BEGIN(graph, 1)

class FFmpegFileMediaGraph : public FileMediaGraph
{
public:
	FFmpegFileMediaGraph(void);
	virtual ~FFmpegFileMediaGraph(void);

protected:
	int openFile(const std::string filePath) override;
	int closeFile(void) override;
	int buildGraph(void) override;
	int getTotalTime(
		long long& hour, long long& minute, long long& second) override;
	int getBitrate(long long& bitrate) override;

private:
	void buildDemuxerFilter(void);
	void buildDecoderFilter(void);

private:
	//AVFormatContext instance.
	void* avFormatContext;
};//class FFmpegFileMediaGraph

NS_END

#endif//FFMPEG_FILE_MEDIA_GRAPH_H
