#include "boost/make_shared.hpp"
#include "error.h"
#include "MediaFilter/Demuxer/AVDemuxerFilter.h"
using AVDemuxerFilter = NS(filter, 1)::AVDemuxerFilter;
#include "MediaFilter/Controller/AVPlayControllerFilter.h"
using AVPlayControllerFilter = NS(filter, 1)::AVPlayControllerFilter;
#include "MediaFilter/Capture/AVCaptureFilter.h"
using AVCaptureFilter = NS(filter, 1)::AVCaptureFilter;
#include "MediaGraph/FFmpeg/FFmpegPlaybackStreamMediaGraph.h"

NS_BEGIN(graph, 1)

FFmpegPlaybackStreamMediaGraph::FFmpegPlaybackStreamMediaGraph() : PlaybackStreamMediaGraph()
{}

FFmpegPlaybackStreamMediaGraph::~FFmpegPlaybackStreamMediaGraph()
{}

int FFmpegPlaybackStreamMediaGraph::openMediaGraph(
	const std::string streamUrl, void* hwnd /* = NULL */, MediaDataCaptureCallback callback /* = NULL */)
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
		createNewMediaCapture(callback);

		// Connect all filters together to start work.
		status = connectPullStreamDataChain();
		if (ERR_OK == status)
		{
			// Start running thread to read stream data.
			runPullStreamDataThread();
		}
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
	MediaFilterPtr mediaDemuxerFilterPtr{ boost::make_shared<AVDemuxerFilter>() };
	boost::shared_ptr<AVDemuxerFilter> avdemuxerFilterPtr{
			boost::dynamic_pointer_cast<AVDemuxerFilter>(mediaDemuxerFilterPtr) };

	if (avdemuxerFilterPtr)
	{
		status = avdemuxerFilterPtr->createNewMediaDemuxer(streamUrl);
		if (ERR_OK == status)
		{
			status = addMediaFilter(NS(filter, 1)::AVMediaDemuxerFilterID, mediaDemuxerFilterPtr);
		}
	}

	return status;
}

int FFmpegPlaybackStreamMediaGraph::createNewMediaController(void)
{
	int status{ ERR_BAD_ALLOC };
	MediaFilterPtr mediaPlayControllerFilterPtr{ boost::make_shared<AVPlayControllerFilter>() };
	boost::shared_ptr<AVPlayControllerFilter> avplaycontrollerFilterPtr{
			boost::dynamic_pointer_cast<AVPlayControllerFilter>(mediaPlayControllerFilterPtr) };

	if (avplaycontrollerFilterPtr)
	{
		status = avplaycontrollerFilterPtr->createNewMediaController();
		if (ERR_OK == status)
		{
			status = addMediaFilter(NS(filter, 1)::AVMediaControllerFilterID, mediaPlayControllerFilterPtr);
		}
	}

	return status;
}

int FFmpegPlaybackStreamMediaGraph::createNewMediaCapture(MediaDataCaptureCallback callback /* = NULL */)
{
	int status{ ERR_BAD_ALLOC };
	MediaFilterPtr mediaCaptureFilterPtr{ boost::make_shared<AVCaptureFilter>() };
	boost::shared_ptr<AVCaptureFilter> avmediaCaptureFilterPtr{
			boost::dynamic_pointer_cast<AVCaptureFilter>(mediaCaptureFilterPtr) };

	if (avmediaCaptureFilterPtr)
	{
		status = avmediaCaptureFilterPtr->createNewMediaCapture(callback);
		if (ERR_OK == status)
		{
			status = addMediaFilter(NS(filter, 1)::AVMediaDataCaptureFilterID, mediaCaptureFilterPtr);
		}
	}

	return status;
}

int FFmpegPlaybackStreamMediaGraph::connectPullStreamDataChain(void)
{
	int status{ ERR_BAD_OPERATE };
	MediaFilterRef mediaDemuxerFilterRef{ queryMediaFilterByID(NS(filter, 1)::AVMediaDemuxerFilterID) },
		mediaPlayControllerFilterRef{ queryMediaFilterByID(NS(filter, 1)::AVMediaControllerFilterID) },
		mediaCaptureFilterRef{ queryMediaFilterByID(NS(filter, 1)::AVMediaDataCaptureFilterID) },
		mediaVideoDecoderFilterRef{ queryMediaFilterByID(NS(filter, 1)::AVMediaVideoDecoderFilterID) },
		mediaAudioDecoderFilterRef{ queryMediaFilterByID(NS(filter, 1)::AVMediaAudioDecoderFilterID) },
		mediaVideoRendererFilterRef{ queryMediaFilterByID(NS(filter, 1)::AVMediaVideoRendererFilterID) },
		mediaAudioRendererFilterRef{ queryMediaFilterByID(NS(filter, 1)::AVMediaAudioRendererFilterID) };

	if (!mediaDemuxerFilterRef.expired() && !mediaPlayControllerFilterRef.expired() && !mediaCaptureFilterRef.expired())
	{
		// If both chain, video and audio, connect fail, media stream can not work.
		boost::shared_ptr<AVDemuxerFilter> avdemuxerFilterPtr{ 
			boost::dynamic_pointer_cast<AVDemuxerFilter>(mediaDemuxerFilterRef.lock()) };
		const MediaStreamID videoStreamID{ avdemuxerFilterPtr->getVideoStreamID() }, audioStreamID{ avdemuxerFilterPtr->getAudioStreamID() };

		if (MediaStreamID::MEDIA_STREAM_ID_NONE != videoStreamID && !mediaVideoDecoderFilterRef.expired() && !mediaVideoRendererFilterRef.expired())
		{
			// Demuxer <-> Controller
			connectOutputPinToInputPin(
				mediaDemuxerFilterRef.lock()->queryMediaPinByID(NS(pin, 1)::VideoStreamOutputPinID),
				mediaPlayControllerFilterRef.lock()->queryMediaPinByID(NS(pin, 1)::VideoStreamInputPinID));
			// Controller <-> Video Decoder
			connectOutputPinToInputPin(
				mediaPlayControllerFilterRef.lock()->queryMediaPinByID(NS(pin, 1)::VideoStreamOutputPinID), 
				mediaVideoDecoderFilterRef.lock()->queryMediaPinByID(NS(pin, 1)::VideoStreamInputPinID));
			// Video Decoder <-> Video renderer
			connectOutputPinToInputPin(
				mediaVideoDecoderFilterRef.lock()->queryMediaPinByID(NS(pin, 1)::VideoStreamOutputPinID), 
				mediaVideoRendererFilterRef.lock()->queryMediaPinByID(NS(pin, 1)::VideoStreamInputPinID));
			// Video renderer <-> Capture
			connectOutputPinToInputPin(
				mediaVideoRendererFilterRef.lock()->queryMediaPinByID(NS(pin, 1)::VideoStreamOutputPinID),
				mediaCaptureFilterRef.lock()->queryMediaPinByID(NS(pin, 1)::MediaStreamInputPinID));

			status = ERR_OK;
		}

		if (MediaStreamID::MEDIA_STREAM_ID_NONE != audioStreamID && !mediaAudioDecoderFilterRef.expired() && !mediaAudioRendererFilterRef.expired())
		{
			// Demuxer <-> Controller
			connectOutputPinToInputPin(
				mediaDemuxerFilterRef.lock()->queryMediaPinByID(NS(pin, 1)::AudioStreamOutputPinID),
				mediaPlayControllerFilterRef.lock()->queryMediaPinByID(NS(pin, 1)::AudioStreamInputPinID));
			// Controller <-> Video Decoder
			connectOutputPinToInputPin(
				mediaPlayControllerFilterRef.lock()->queryMediaPinByID(NS(pin, 1)::AudioStreamOutputPinID),
				mediaVideoDecoderFilterRef.lock()->queryMediaPinByID(NS(pin, 1)::AudioStreamInputPinID));
			// Video Decoder <-> Video renderer
			connectOutputPinToInputPin(
				mediaVideoDecoderFilterRef.lock()->queryMediaPinByID(NS(pin, 1)::AudioStreamOutputPinID),
				mediaVideoRendererFilterRef.lock()->queryMediaPinByID(NS(pin, 1)::AudioStreamInputPinID));
			// Video renderer <-> Capture
			connectOutputPinToInputPin(
				mediaVideoRendererFilterRef.lock()->queryMediaPinByID(NS(pin, 1)::AudioStreamOutputPinID),
				mediaCaptureFilterRef.lock()->queryMediaPinByID(NS(pin, 1)::MediaStreamInputPinID));

			status = ERR_OK;
		}
	}

	return status;
}

int FFmpegPlaybackStreamMediaGraph::connectOutputPinToInputPin(MediaPinRef outputPin, MediaPinRef inputPin)
{
	return !outputPin.expired() && !inputPin.expired() ? outputPin.lock()->connectPin(inputPin) : ERR_BAD_OPERATE;
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
