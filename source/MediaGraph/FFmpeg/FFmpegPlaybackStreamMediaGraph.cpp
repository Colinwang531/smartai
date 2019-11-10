#include "boost/make_shared.hpp"
#include "error.h"
#include "MediaFilter/Demuxer/AVDemuxerFilter.h"
using AVDemuxerFilter = NS(filter, 1)::AVDemuxerFilter;
#include "MediaFilter/Controller/AVPlayControllerFilter.h"
using AVPlayControllerFilter = NS(filter, 1)::AVPlayControllerFilter;
#include "MediaGraph/FFmpeg/FFmpegPlaybackStreamMediaGraph.h"

NS_BEGIN(graph, 1)

FFmpegPlaybackStreamMediaGraph::FFmpegPlaybackStreamMediaGraph() : PlaybackStreamMediaGraph()
{}

FFmpegPlaybackStreamMediaGraph::~FFmpegPlaybackStreamMediaGraph()
{}

int FFmpegPlaybackStreamMediaGraph::openMediaGraph(const std::string streamUrl, void* hwnd /* = NULL */)
{
	int status{ !streamUrl.empty() ? ERR_OK : ERR_INVALID_PARAM };

	if (ERR_OK == status)
	{
		// Demuxer
		createNewMediaDemuxer(streamUrl);
		// Controller
		createNewMediaController();
		// Decoder.
		createNewVideoDecoderFilter();
		createNewAudioDecoderFilter();
		// Renderer
		createNewVideoRendererFilter(hwnd);
		createNewAudioRendererFilter();
		// Grabber
		// Start running thread to read stream data.
		runPullStreamDataThread();
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

int FFmpegPlaybackStreamMediaGraph::createNewMediaController(void)
{
	int status{ ERR_BAD_ALLOC };
	MediaFilterPtr avPlayControllerFilterPtr{ boost::make_shared<AVPlayControllerFilter>() };
	boost::shared_ptr<AVPlayControllerFilter> avplaycontrollerFilterPtr{
			boost::dynamic_pointer_cast<AVPlayControllerFilter>(avPlayControllerFilterPtr) };

	if (avplaycontrollerFilterPtr)
	{
		status = avplaycontrollerFilterPtr->createNewMediaController();
		if (ERR_OK == status)
		{
			addMediaFilter(NS(filter, 1)::AVMediaControllerFilterID, avPlayControllerFilterPtr);
		}
	}

	return status;
}

int FFmpegPlaybackStreamMediaGraph::runPullStreamDataThread(void)
{
	int status{ ERR_NOT_SUPPORT };
	MediaFilterPtr demuxerFilterPtr{ queryMediaFilterByID(NS(filter, 1)::AVMediaDemuxerFilterID) };
	
	if (demuxerFilterPtr)
	{
		boost::shared_ptr<AVDemuxerFilter> avdemuxerFilterPtr{
			boost::dynamic_pointer_cast<AVDemuxerFilter>(demuxerFilterPtr) };
		status = avdemuxerFilterPtr->runPullStreamDataThread();
	}

	return status;
}

NS_END
