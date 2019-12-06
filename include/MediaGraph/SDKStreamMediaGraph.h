// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Base class of graph for live stream.
//

#ifndef SDK_STREAM_MEDIA_GRAPH_H
#define SDK_STREAM_MEDIA_GRAPH_H

#include "MediaGraph.h"

namespace framework
{
	namespace multimedia
	{
		using MediaGraphPtr = boost::shared_ptr<MediaGraph>;

		class SDKStreamMediaGraph : public MediaGraph
		{
		public:
			SDKStreamMediaGraph(void);
			virtual ~SDKStreamMediaGraph(void);

		protected:
			int createNewGraph(void) override;
			virtual int createNewDataCaptureFilter(void);
			virtual int createNewSDKDecoderFilter(void);

		private:
			void buildMediaGraph(void);

		private:
			MediaGraphPtr foundationalMediaGraphPtr;
		};//class SDKStreamMediaGraph
	}//namespace multimedia
}//namespace framework

#endif//SDK_STREAM_MEDIA_GRAPH_H
