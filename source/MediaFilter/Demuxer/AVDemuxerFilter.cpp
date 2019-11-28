#include "boost/make_shared.hpp"
#include "MediaPin/MediaPin.h"
#include "MediaModel/Demuxer/FFmpeg/FFmpegLocalFileDemuxer.h"
#include "MediaFilter/Demuxer/AVDemuxerFilter.h"

namespace framework
{
	namespace multimedia
	{
		AVDemuxerFilter::AVDemuxerFilter() : SourceMediaFilter()
		{}

		AVDemuxerFilter::~AVDemuxerFilter()
		{}

		int AVDemuxerFilter::createNewFilter(
			const MediaStreamID mediaStreamID /* = MediaStreamID::MEDIA_STREAM_ID_AV */)
		{
			if (MediaStreamID::MEDIA_STREAM_ID_AV == mediaStreamID || MediaStreamID::MEDIA_STREAM_ID_VIDEO == mediaStreamID)
			{
				createNewOutputPin(VideoStreamOutputPinID);
			}
			if (MediaStreamID::MEDIA_STREAM_ID_AV == mediaStreamID || MediaStreamID::MEDIA_STREAM_ID_AUDIO == mediaStreamID)
			{
				createNewOutputPin(AudioStreamOutputPinID);
			}

			MediaModelPtr ffmpegLocalFileDemuxerPtr{ boost::make_shared<FFmpegLocalFileDemuxer>() };
			if (ffmpegLocalFileDemuxerPtr)
			{
				mediaModelPtr.swap(ffmpegLocalFileDemuxerPtr);
			}

			return MediaFilter::createNewFilter(mediaStreamID);
		}
	}//namespace multimedia
}//namespace framework
