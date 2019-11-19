#include "boost/bind.hpp"
#include "boost/make_shared.hpp"
#include "error.h"
#include "MediaFilter/Demuxer/AVDemuxerFilter.h"
using AVDemuxerFilter = NS(filter, 1)::AVDemuxerFilter;
#include "MediaFilter/Controller/AVPlayControllerFilter.h"
using AVPlayControllerFilter = NS(filter, 1)::AVPlayControllerFilter;
#include "MediaFilter/Capture/AVCaptureFilter.h"
using AVCaptureFilter = NS(filter, 1)::AVCaptureFilter;
#include "MediaFilter/Decoder/AVDecoderFilter.h"
using AVDecoderFilter = NS(filter, 1)::AVDecoderFilter;
#include "MediaFilter/Renderer/AVRendererFilter.h"
using AVRendererFilter = NS(filter, 1)::AVRendererFilter;
#include "MediaGraph/PlaybackMediaGraph.h"

NS_BEGIN(graph, 1)

PlaybackMediaGraph::PlaybackMediaGraph() : MediaGraph()
{}

PlaybackMediaGraph::~PlaybackMediaGraph()
{}

int PlaybackMediaGraph::createNewGraph()
{
	return ERR_OK == createNewDemuxerFilter() && 
		ERR_OK == createNewControllerFilter() &&
		ERR_OK == createNewVideoDecoderFilter() &&
		ERR_OK == createNewVideoRendererFilter() &&
		ERR_OK == createNewAudioDecoderFilter() &&
		ERR_OK == createNewAudioRendererFilter() &&
		ERR_OK == createNewDataCaptureFilter()
		? linkMediaGraph() : destroyGraph();
}

int PlaybackMediaGraph::destroyGraph()
{
	return ERR_OK;
}

int PlaybackMediaGraph::linkMediaGraph()
{
	int status{ ERR_BAD_OPERATE };
	MediaFilterRef demuxerFilterRef{ queryMediaFilterByID(NS(filter, 1)::AVMediaDemuxerFilterID) },
		controllerFilterRef{ queryMediaFilterByID(NS(filter, 1)::AVMediaControllerFilterID) },
		videoDecoderFilterRef{ queryMediaFilterByID(NS(filter, 1)::AVMediaVideoDecoderFilterID) },
		audioDecoderFilterRef{ queryMediaFilterByID(NS(filter, 1)::AVMediaAudioDecoderFilterID) },
		videoRendererFilterRef{ queryMediaFilterByID(NS(filter, 1)::AVMediaVideoRendererFilterID) },
		audioRendererFilterRef{ queryMediaFilterByID(NS(filter, 1)::AVMediaAudioRendererFilterID) },
		dataCaptureFilterRef{ queryMediaFilterByID(NS(filter, 1)::AVMediaDataCaptureFilterID) };

	if (!demuxerFilterRef.expired() && !controllerFilterRef.expired())
	{
		// Demuxer->Controller(video)
		MediaPinRef demuxerVideoOutputPinRef{ demuxerFilterRef.lock()->queryMediaPinByID(NS(pin, 1)::VideoStreamOutputPinID) };
		MediaPinRef controllerVideoInputPinRef{ controllerFilterRef.lock()->queryMediaPinByID(NS(pin, 1)::VideoStreamInputPinID) };
		if (!demuxerVideoOutputPinRef.expired() && !controllerVideoInputPinRef.expired())
		{
			demuxerVideoOutputPinRef.lock()->connectPin(controllerVideoInputPinRef);
		}

		// Demuxer->Controller(audio)
		MediaPinRef demuxerAudioOutputPinRef{ demuxerFilterRef.lock()->queryMediaPinByID(NS(pin, 1)::AudioStreamOutputPinID) };
		MediaPinRef controllerAudioInputPinRef{ controllerFilterRef.lock()->queryMediaPinByID(NS(pin, 1)::AudioStreamInputPinID) };
		if (!demuxerAudioOutputPinRef.expired() && !controllerAudioInputPinRef.expired())
		{
			demuxerAudioOutputPinRef.lock()->connectPin(controllerAudioInputPinRef);
		}

		//Controller->Decoder->Renderer(video)
		if (!videoDecoderFilterRef.expired() && !videoRendererFilterRef.expired())
		{
			MediaPinRef controllerVideoOutputPinRef{ controllerFilterRef.lock()->queryMediaPinByID(NS(pin, 1)::VideoStreamOutputPinID) };
			MediaPinRef videoDecoderInputPinRef{ videoDecoderFilterRef.lock()->queryMediaPinByID(NS(pin, 1)::VideoStreamInputPinID) };
			MediaPinRef videoDecoderOutputPinRef{ videoDecoderFilterRef.lock()->queryMediaPinByID(NS(pin, 1)::VideoStreamOutputPinID) };
			MediaPinRef videoRendererInputPinRef{ videoRendererFilterRef.lock()->queryMediaPinByID(NS(pin, 1)::VideoStreamInputPinID) };
			if (!controllerVideoOutputPinRef.expired() && !videoDecoderInputPinRef.expired() && 
				!videoDecoderOutputPinRef.expired() && !videoRendererInputPinRef.expired())
			{
				controllerVideoOutputPinRef.lock()->connectPin(videoDecoderInputPinRef);
				videoDecoderOutputPinRef.lock()->connectPin(videoRendererInputPinRef);
			}
		}

		//Controller->Decoder->Renderer(audio)
		if (!audioDecoderFilterRef.expired() && !audioRendererFilterRef.expired())
		{
			MediaPinRef controllerAudioOutputPinRef{ controllerFilterRef.lock()->queryMediaPinByID(NS(pin, 1)::AudioStreamOutputPinID) };
			MediaPinRef audioDecoderInputPinRef{ audioDecoderFilterRef.lock()->queryMediaPinByID(NS(pin, 1)::AudioStreamInputPinID) };
			MediaPinRef audioDecoderOutputPinRef{ audioDecoderFilterRef.lock()->queryMediaPinByID(NS(pin, 1)::AudioStreamOutputPinID) };
			MediaPinRef audioRendererInputPinRef{ audioRendererFilterRef.lock()->queryMediaPinByID(NS(pin, 1)::AudioStreamInputPinID) };
			if (!controllerAudioOutputPinRef.expired() && !audioDecoderInputPinRef.expired() &&
				!audioDecoderOutputPinRef.expired() && !audioRendererInputPinRef.expired())
			{
				controllerAudioOutputPinRef.lock()->connectPin(audioDecoderInputPinRef);
				audioDecoderOutputPinRef.lock()->connectPin(audioRendererInputPinRef);
			}
		}

		if (!dataCaptureFilterRef.expired())
		{
			// Renderer->Capture(video)
			MediaPinRef rendererVideoOutputPinRef{ videoRendererFilterRef.lock()->queryMediaPinByID(NS(pin, 1)::VideoStreamOutputPinID) };
			MediaPinRef dataCaptureInputPinRef{ dataCaptureFilterRef.lock()->queryMediaPinByID(NS(pin, 1)::VideoStreamInputPinID) };
			if (!rendererVideoOutputPinRef.expired() && !dataCaptureInputPinRef.expired())
			{
				rendererVideoOutputPinRef.lock()->connectPin(dataCaptureInputPinRef);
			}

			//Renderer->Capture(audio)
			MediaPinRef rendererAudioOutputPinRef{ audioRendererFilterRef.lock()->queryMediaPinByID(NS(pin, 1)::AudioStreamInputPinID) };
			if (!rendererAudioOutputPinRef.expired() && !dataCaptureInputPinRef.expired())
			{
				rendererAudioOutputPinRef.lock()->connectPin(dataCaptureInputPinRef);
			}
		}

		status = ERR_OK;
	}

	return status;
}

int PlaybackMediaGraph::createNewDemuxerFilter()
{
	int status{ ERR_BAD_ALLOC };
	MediaFilterPtr mediaFilterPtr{ boost::make_shared<AVDemuxerFilter>() };

	if (mediaFilterPtr && ERR_OK == mediaFilterPtr->createNewFilter())
	{
		status = addMediaFilter(NS(filter, 1)::AVMediaDemuxerFilterID, mediaFilterPtr);
	}

	return status;
}

int PlaybackMediaGraph::createNewControllerFilter(void)
{
	int status{ ERR_BAD_ALLOC };
	MediaFilterPtr mediaFilterPtr{ boost::make_shared<AVPlayControllerFilter>() };

	if (mediaFilterPtr && ERR_OK == mediaFilterPtr->createNewFilter())
	{
		status = addMediaFilter(NS(filter, 1)::AVMediaControllerFilterID, mediaFilterPtr);
	}

	return status;
}

int PlaybackMediaGraph::createNewVideoDecoderFilter()
{
	int status{ ERR_BAD_ALLOC };
	MediaFilterPtr mediaFilterPtr{ 
		boost::make_shared<AVDecoderFilter>(NS(filter, 1)::AVDecoderType::AV_DECODER_TYPE_VIDEO) };

	if (mediaFilterPtr && ERR_OK == mediaFilterPtr->createNewFilter())
	{
		status = addMediaFilter(NS(filter, 1)::AVMediaVideoDecoderFilterID, mediaFilterPtr);
	}

	return status;
}

int PlaybackMediaGraph::createNewAudioDecoderFilter()
{
	int status{ ERR_BAD_ALLOC };
	MediaFilterPtr mediaFilterPtr{ 
		boost::make_shared<AVDecoderFilter>(NS(filter, 1)::AVDecoderType::AV_DECODER_TYPE_AUDIO) };

	if (mediaFilterPtr && ERR_OK == mediaFilterPtr->createNewFilter())
	{
		status = addMediaFilter(NS(filter, 1)::AVMediaAudioDecoderFilterID, mediaFilterPtr);
	}

	return status;
}

int PlaybackMediaGraph::createNewVideoRendererFilter()
{
	int status{ ERR_BAD_ALLOC };
	MediaFilterPtr mediaFilterPtr{ 
		boost::make_shared<AVRendererFilter>(NS(filter, 1)::AVRendererType::AV_RENDERER_TYPE_VIDEO) };

	if (mediaFilterPtr && mediaFilterPtr->createNewFilter())
	{
		status = addMediaFilter(NS(filter, 1)::AVMediaVideoRendererFilterID, mediaFilterPtr);
	}

	return status;
}

int PlaybackMediaGraph::createNewAudioRendererFilter(void)
{
	int status{ ERR_BAD_ALLOC };
	MediaFilterPtr mediaFilterPtr{ 
		boost::make_shared<AVRendererFilter>(NS(filter, 1)::AVRendererType::AV_RENDERER_TYPE_AUDIO) };

	if (mediaFilterPtr && mediaFilterPtr->createNewFilter())
	{
		status = addMediaFilter(NS(filter, 1)::AVMediaAudioRendererFilterID, mediaFilterPtr);
	}

	return status;
}

int PlaybackMediaGraph::createNewDataCaptureFilter()
{
	int status{ ERR_BAD_ALLOC };
	MediaFilterPtr mediaFilterPtr{ boost::make_shared<AVCaptureFilter>() };

	if (mediaFilterPtr && mediaFilterPtr->createNewFilter())
	{
		status = addMediaFilter(NS(filter, 1)::AVMediaDataCaptureFilterID, mediaFilterPtr);
	}

	return status;
}

NS_END
