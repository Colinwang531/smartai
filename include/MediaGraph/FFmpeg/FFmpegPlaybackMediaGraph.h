// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Base class of graph for disposing local file using FFmpeg library.
//

#ifndef FFMPEG_PLAYBACK_MEDIA_GRAPH_H
#define FFMPEG_PLAYBACK_MEDIA_GRAPH_H

#include "MediaGraph/PlaybackMediaGraph.h"

namespace framework
{
	namespace multimedia
	{
		class FFmpegPlaybackMediaGraph : public PlaybackMediaGraph
		{
		public:
			FFmpegPlaybackMediaGraph(void);
			virtual ~FFmpegPlaybackMediaGraph(void);

		protected:
			int createNewDemuxerFilter(void) override;
			int createNewImageFormatterFilter(void) override;
			int createNewVideoDecoderFilter(void) override;
			int createNewAudioDecoderFilter(void) override;
			int createNewVideoRendererFilter(void) override;
			int createNewSoundPlayerFilter(void) override;
		};//class FFmpegPlaybackMediaGraph
	}//namespace multimedia
}//namespace framework

#endif//FFMPEG_PLAYBACK_MEDIA_GRAPH_H
