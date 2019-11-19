// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Base class of graph for live stream.
//

#ifndef LIVESTREAM_MEDIA_GRAPH_H
#define LIVESTREAM_MEDIA_GRAPH_H

#include "MediaGraph.h"

NS_BEGIN(graph, 1)

class LivestreamMediaGraph : public MediaGraph
{
public:
	LivestreamMediaGraph(void);
	virtual ~LivestreamMediaGraph(void);

protected:
	int createNewGraph(void) override;
	int destroyGraph(void) override;
	int linkMediaGraph(void) override;

protected:
	// Decoder and renderer filters were created by user.
	virtual int createNewVideoDecoderFilter(void);
	virtual int createNewAudioDecoderFilter(void);
	virtual int createNewVideoRendererFilter(void);
	virtual int createNewAudioRendererFilter(void);

private:
	// Live stream and callback data capture filters were created by creator.
	virtual int createNewLivestreamCaptureFilter(void);
	virtual int createNewDataCaptureFilter(void);
};//class LivestreamMediaGraph

NS_END

#endif//LIVESTREAM_MEDIA_GRAPH_H
