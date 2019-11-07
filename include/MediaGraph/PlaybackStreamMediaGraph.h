// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Base class of graph for playback stream.
//

#ifndef PLAYBACK_STREAM_MEDIA_GRAPH_H
#define PLAYBACK_STREAM_MEDIA_GRAPH_H

#include "MediaGraph.h"

NS_BEGIN(graph, 1)

class PlaybackStreamMediaGraph : public MediaGraph
{
public:
	PlaybackStreamMediaGraph(void);
	virtual ~PlaybackStreamMediaGraph(void);

protected:
	int createNewVideoDecoderFilter(void);
	int createNewAudioDecoderFilter(void);

// public:
// 	virtual const long long getTotalSeconds(void) = 0;
// 	virtual const long long getBitrate(void) = 0;
// 
// protected:
// 	int startPlay(void) override;
// 	int stopPlay(void) override;
// 	int pausePlay(void) override;
// 	int fastPlay(const short speed = 1) override;
// 	int slowPlay(const short speed = -1) override;
// 
// private:
// 	int checkSpeedValue(const short speed = 0);
};//class PlaybackStreamMediaGraph

NS_END

#endif//PLAYBACK_STREAM_MEDIA_GRAPH_H
