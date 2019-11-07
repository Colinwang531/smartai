#include "boost/make_shared.hpp"
#include "error.h"
#include "MediaFilter/Demuxer/AVDemuxerFilter.h"
using AVDemuxerFilter = NS(filter, 1)::AVDemuxerFilter;
#include "MediaGraph/FFmpeg/FFmpegPlaybackStreamMediaGraph.h"

NS_BEGIN(graph, 1)

FFmpegPlaybackStreamMediaGraph::FFmpegPlaybackStreamMediaGraph() : PlaybackStreamMediaGraph()
{}

FFmpegPlaybackStreamMediaGraph::~FFmpegPlaybackStreamMediaGraph()
{}

int FFmpegPlaybackStreamMediaGraph::openMediaGraph(const std::string streamUrl)
{
	int status{ !streamUrl.empty() ? ERR_OK : ERR_INVALID_PARAM };

	if (ERR_OK == status)
	{
		// Demuxer
		createNewMediaDemuxer(streamUrl);
		// Decoder.
		createNewVideoDecoderFilter(demuxerFilterPtr->getVideoStreamID());
		createNewAudioDecoderFilter(demuxerFilterPtr->getAudioStreamID());

		// Start running thread to read stream data.

	}

	return status;
}

int FFmpegPlaybackStreamMediaGraph::closeMediaGraph()
{
	return ERR_OK;
}

int FFmpegPlaybackStreamMediaGraph::createNewMediaDemuxer(const std::string streamUrl)
{
	int status{ ERR_BAD_ALLOC };
	MediaFilterPtr avdemuxerFilterPtr{ boost::make_shared<AVDemuxerFilter>() };
	boost::shared_ptr<AVDemuxerFilter> demuxerFilterPtr{
			boost::dynamic_pointer_cast<AVDemuxerFilter>(avdemuxerFilterPtr) };

	if (demuxerFilterPtr)
	{
		status = demuxerFilterPtr->createNewMediaDemuxer(streamUrl);
		if (ERR_OK == status)
		{
			addMediaFilter(NS(filter, 1)::AVMediaDemuxerFilterID, avdemuxerFilterPtr);
		}
	}

	return status;
}

NS_END
