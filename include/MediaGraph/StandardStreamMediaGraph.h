// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// The standard component of media graph includes data capture, 
// play controller, video decoder and audio decoder.
//

#ifndef STANDARD_STREAM_MEDIA_GRAPH_H
#define STANDARD_STREAM_MEDIA_GRAPH_H

#include "MediaGraph.h"

namespace framework
{
	namespace multimedia
	{
		using MediaGraphPtr = boost::shared_ptr<MediaGraph>;

		class StandardStreamMediaGraph : public MediaGraph
		{
		public:
			StandardStreamMediaGraph(void);
			virtual ~StandardStreamMediaGraph(void);

		protected:
			int createNewGraph(void) override;
			virtual int createNewDataCaptureFilter(void);
			virtual int createNewPlayControllerFilter(void);
			virtual int createNewVideoDecoderFilter(void);
			virtual int createNewAudioDecoderFilter(void);

		private:
			void buildMediaGraph(void);

		private:
			MediaGraphPtr foundationalMediaGraphPtr;
		};//class StandardStreamMediaGraph
	}//namespace multimedia
}//namespace framework

#endif//STANDARD_STREAM_MEDIA_GRAPH_H
