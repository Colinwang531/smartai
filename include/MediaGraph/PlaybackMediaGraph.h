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

#include "AVPlayMediaGraph.h"

namespace framework
{
	namespace multimedia
	{
		class PlaybackMediaGraph : public AVPlayMediaGraph
		{
		public:
			PlaybackMediaGraph(void);
			virtual ~PlaybackMediaGraph(void);

		protected:
			int createNewGraph(void) override;
			int buildMediaGraph(void) override;
			virtual int createNewDemuxerFilter(void);
			virtual int createNewPlayControllerFilter(void);
		};//class PlaybackMediaGraph
	}//namespace multimedia
}//namespace framework

#endif//PLAYBACK_MEDIA_GRAPH_H
