#include "boost/make_shared.hpp"
#include "error.h"
#include "MediaFilter/Demuxer/AVDemuxerFilter.h"
#include "MediaFilter/Formatter/ImageFormatterFilter.h"
#include "MediaFilter/Decoder/AVDecoderFilter.h"
#include "MediaFilter/Renderer/AVRendererFilter.h"
#include "MediaGraph/FFmpeg/FFmpegPlaybackMediaGraph.h"

namespace framework
{
	namespace multimedia
	{
		FFmpegPlaybackMediaGraph::FFmpegPlaybackMediaGraph() : PlaybackMediaGraph()
		{}

		FFmpegPlaybackMediaGraph::~FFmpegPlaybackMediaGraph()
		{}

		int FFmpegPlaybackMediaGraph::createNewDemuxerFilter(void)
		{
			int status{ ERR_BAD_ALLOC };
			MediaFilterPtr mediaFilterPtr{ boost::make_shared<AVDemuxerFilter>() };

			if (mediaFilterPtr && ERR_OK == mediaFilterPtr->createNewFilter(MediaStreamID::MEDIA_STREAM_ID_AV))
			{
				status = addMediaFilter(AVMediaDemuxerFilterID, mediaFilterPtr);
			}

			return status;
		}

		int FFmpegPlaybackMediaGraph::createNewImageFormatterFilter(void)
		{
			int status{ ERR_BAD_ALLOC };
			MediaFilterPtr mediaFilterPtr{ boost::make_shared<ImageFormatterFilter>() };

			if (mediaFilterPtr && ERR_OK == mediaFilterPtr->createNewFilter(MediaStreamID::MEDIA_STREAM_ID_VIDEO))
			{
				status = addMediaFilter(AVMediaImageFormatFilterID, mediaFilterPtr);
			}

			return status;
		}

		int FFmpegPlaybackMediaGraph::createNewVideoDecoderFilter(void)
		{
			int status{ ERR_BAD_ALLOC };
			MediaFilterPtr mediaFilterPtr{ boost::make_shared<AVDecoderFilter>() };

			if (mediaFilterPtr && ERR_OK == mediaFilterPtr->createNewFilter(MediaStreamID::MEDIA_STREAM_ID_VIDEO))
			{
				status = addMediaFilter(AVMediaVideoDecoderFilterID, mediaFilterPtr);
			}

			return status;
		}

		int FFmpegPlaybackMediaGraph::createNewAudioDecoderFilter(void)
		{
			int status{ ERR_BAD_ALLOC };
			MediaFilterPtr mediaFilterPtr{ boost::make_shared<AVDecoderFilter>() };

			if (mediaFilterPtr && ERR_OK == mediaFilterPtr->createNewFilter(MediaStreamID::MEDIA_STREAM_ID_AUDIO))
			{
				status = addMediaFilter(AVMediaAudioDecoderFilterID, mediaFilterPtr);
			}

			return status;
		}

		int FFmpegPlaybackMediaGraph::createNewVideoRendererFilter(void)
		{
			int status{ ERR_BAD_ALLOC };
			MediaFilterPtr mediaFilterPtr{ boost::make_shared<AVRendererFilter>() };

			if (mediaFilterPtr && ERR_OK == mediaFilterPtr->createNewFilter(MediaStreamID::MEDIA_STREAM_ID_VIDEO))
			{
				status = addMediaFilter(AVMediaVideoRendererFilterID, mediaFilterPtr);
			}

			return status;
		}

		int FFmpegPlaybackMediaGraph::createNewSoundPlayerFilter(void)
		{
			int status{ ERR_BAD_ALLOC };
			MediaFilterPtr mediaFilterPtr{ boost::make_shared<AVRendererFilter>() };

			if (mediaFilterPtr && ERR_OK == mediaFilterPtr->createNewFilter(MediaStreamID::MEDIA_STREAM_ID_AUDIO))
			{
				status = addMediaFilter(AVMediaSoundPlayerFilterID, mediaFilterPtr);
			}

			return status;
		}
	}//namespace multimedia
}//namespace framework
