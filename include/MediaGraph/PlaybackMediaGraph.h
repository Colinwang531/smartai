// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Base class of graph for playback stream.
//

#ifndef PLAYBACK_MEDIA_GRAPH_H
#define PLAYBACK_MEDIA_GRAPH_H

#include "MediaGraph.h"

NS_BEGIN(graph, 1)

class PlaybackMediaGraph : public MediaGraph
{
public:
	PlaybackMediaGraph(void);
	virtual ~PlaybackMediaGraph(void);

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
	// Demuxer, controller and callback data capture filters were created by creator.
	virtual int createNewDemuxerFilter(void);
	virtual int createNewControllerFilter(void);
	virtual int createNewDataCaptureFilter(void);
};//class PlaybackMediaGraph

NS_END

#endif//PLAYBACK_MEDIA_GRAPH_H
