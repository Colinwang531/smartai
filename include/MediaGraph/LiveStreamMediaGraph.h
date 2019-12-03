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

#include "AVPlayMediaGraph.h"

namespace framework
{
	namespace multimedia
	{
		class LivestreamMediaGraph : public AVPlayMediaGraph
		{
		public:
			LivestreamMediaGraph(void);
			virtual ~LivestreamMediaGraph(void);

		protected:
			int createNewGraph(void) override;
			int buildMediaGraph(void) override;
			virtual int createNewLivestreamCaptureFilter(void);
		};//class LivestreamMediaGraph
	}//namespace multimedia
}//namespace framework

#endif//LIVESTREAM_MEDIA_GRAPH_H
