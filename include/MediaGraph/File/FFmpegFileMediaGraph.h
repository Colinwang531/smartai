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
	int inputMediaData(MediaDataPtr mediaData) override;
	unsigned long long getTotalSeconds(void) override;
	unsigned long long getBitrate(void) override;

private:
	int createNewDemuxerFilter(MediaDataPtr mediaData);
	void buildDecoderFilter(void);
};//class FFmpegFileMediaGraph

NS_END

#endif//FFMPEG_FILE_MEDIA_GRAPH_H
