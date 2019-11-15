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
	int createNewGraph(void* hwnd = NULL, void* callback = NULL) override;
	int destroyGraph(void) override;

protected:
	virtual int createNewVideoDecoderFilter(
		const MediaDataSubID videoSubID = MediaDataSubID::MEDIA_DATA_SUB_ID_NONE);
	virtual int createNewAudioDecoderFilter(
		const MediaDataSubID audioSubID = MediaDataSubID::MEDIA_DATA_SUB_ID_NONE);
	virtual int createNewVideoRendererFilter(void);
	virtual int createNewAudioRendererFilter(void);

private:
	int createNewDemuxerFilter(void);
	int createNewControllerFilter(void);
	int createNewCaptureFilter(void* callback = NULL);
	int notifyMediaDataSubIDCallback(
		const MediaDataSubID videoSubID = MediaDataSubID::MEDIA_DATA_SUB_ID_NONE,
		const MediaDataSubID audioSubID = MediaDataSubID::MEDIA_DATA_SUB_ID_NONE);
};//class PlaybackMediaGraph

NS_END

#endif//PLAYBACK_MEDIA_GRAPH_H
