// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Base class of graph for operating video and audio stream.
//

#ifndef AV_PLAY_MEDIA_GRAPH_H
#define AV_PLAY_MEDIA_GRAPH_H

#include "MediaGraph.h"

namespace framework
{
	namespace multimedia
	{
		class AVPlayMediaGraph : public MediaGraph
		{
		public:
			AVPlayMediaGraph(void);
			virtual ~AVPlayMediaGraph(void);

		protected:
			int createNewGraph(void) override;
			virtual int buildMediaGraph(void);
			virtual int createNewImageFormatterFilter(void);
			virtual int createNewVideoDecoderFilter(void);
			virtual int createNewAudioDecoderFilter(void);
			virtual int createNewVideoRendererFilter(void);
			virtual int createNewSoundPlayerFilter(void);
			virtual int createNewDataCaptureFilter(void);
		};//class AVPlayMediaGraph
	}//namespace multimedia
}//namespace framework

#endif//AV_PLAY_MEDIA_GRAPH_H
