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

namespace framework
{
	namespace multimedia
	{
		class PlaybackMediaGraph : public MediaGraph
		{
		public:
			PlaybackMediaGraph(void);
			virtual ~PlaybackMediaGraph(void);

		protected:
			int createNewGraph(void) override;
			int postCreateNewGraph(void) override;

		protected:
			// Filters of demuxer, decoder, image formatter and renderer were created by user.
			virtual int createNewDemuxerFilter(void) = 0;
			virtual int createNewImageFormatterFilter(void) = 0;
			virtual int createNewVideoDecoderFilter(void) = 0;
			virtual int createNewAudioDecoderFilter(void) = 0;
			virtual int createNewVideoRendererFilter(void) = 0;
			virtual int createNewSoundPlayerFilter(void) = 0;

		private:
			// Filters of Play controller and data capture were created by creator.
			virtual int createNewPlayControllerFilter(void);
			virtual int createNewDataCaptureFilter(void);
			// Connect all filters.
			int linkMediaFilter(void);
		};//class PlaybackMediaGraph
	}//namespace multimedia
}//namespace framework

#endif//PLAYBACK_MEDIA_GRAPH_H
