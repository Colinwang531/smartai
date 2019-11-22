#include "boost/bind.hpp"
#include "boost/make_shared.hpp"
#include "error.h"
#include "MediaFilter/Capture/LivestreamCaptureFilter.h"
using LivestreamCaptureFilter = NS(filter, 1)::LivestreamCaptureFilter;
#include "MediaFilter/Capture/AVCaptureFilter.h"
using AVCaptureFilter = NS(filter, 1)::AVCaptureFilter;
#include "MediaFilter/Decoder/AVDecoderFilter.h"
using AVDecoderFilter = NS(filter, 1)::AVDecoderFilter;
#include "MediaFilter/Renderer/AVRendererFilter.h"
using AVRendererFilter = NS(filter, 1)::AVRendererFilter;
#include "MediaFilter/Formatter/VideoFormatterFilter.h"
using VideoFormatterFilter = NS(filter, 1)::VideoFormatterFilter;
#include "MediaGraph/LivestreamMediaGraph.h"

NS_BEGIN(graph, 1)

LivestreamMediaGraph::LivestreamMediaGraph() : MediaGraph()
{}

LivestreamMediaGraph::~LivestreamMediaGraph()
{}

int LivestreamMediaGraph::createNewGraph()
{
	return ERR_OK == createNewLivestreamCaptureFilter() &&
		ERR_OK == createNewVideoDecoderFilter() &&
		ERR_OK == createNewVideoRendererFilter() &&
		ERR_OK == createNewAudioDecoderFilter() &&
		ERR_OK == createNewAudioRendererFilter() &&
		ERR_OK == createNewDataCaptureFilter() &&
		ERR_OK == createNewVideoFormatterFilter()
		? linkMediaGraph() : destroyGraph();
}

int LivestreamMediaGraph::destroyGraph()
{
	return ERR_OK;
}

int LivestreamMediaGraph::linkMediaGraph()
{
	int status{ ERR_BAD_OPERATE };
	MediaFilterRef livestreamCaptureFilterRef{ queryMediaFilterByID(NS(filter, 1)::AVMediaLivestreamCaptureFilterID) },
		videoDecoderFilterRef{ queryMediaFilterByID(NS(filter, 1)::AVMediaVideoDecoderFilterID) },
		audioDecoderFilterRef{ queryMediaFilterByID(NS(filter, 1)::AVMediaAudioDecoderFilterID) },
		videoRendererFilterRef{ queryMediaFilterByID(NS(filter, 1)::AVMediaVideoRendererFilterID) },
		audioRendererFilterRef{ queryMediaFilterByID(NS(filter, 1)::AVMediaAudioRendererFilterID) },
		dataCaptureFilterRef{ queryMediaFilterByID(NS(filter, 1)::AVMediaDataCaptureFilterID) },
		videoFormatterFilterRef{ queryMediaFilterByID(NS(filter, 1)::AVMediaVideoFormatterFilterID) };

	if (!livestreamCaptureFilterRef.expired())
	{
		//Live stream capture->Decoder->Capture(HIKIVISION video)
		if (!videoDecoderFilterRef.expired() /*&& !videoRendererFilterRef.expired()*/ && !dataCaptureFilterRef.expired() && !videoFormatterFilterRef.expired())
		{
			MediaPinRef livestreamVideoOutputPinRef{ livestreamCaptureFilterRef.lock()->queryMediaPinByID(NS(pin, 1)::VideoStreamOutputPinID) };
			MediaPinRef videoDecoderInputPinRef{ videoDecoderFilterRef.lock()->queryMediaPinByID(NS(pin, 1)::VideoStreamInputPinID) };
			MediaPinRef videoDecoderOutputPinRef{ videoDecoderFilterRef.lock()->queryMediaPinByID(NS(pin, 1)::VideoStreamOutputPinID) };
			MediaPinRef videoFormatterInputPinRef{ videoFormatterFilterRef.lock()->queryMediaPinByID(NS(pin, 1)::VideoStreamInputPinID) };
			MediaPinRef videoFormatterOutputPinRef{ videoFormatterFilterRef.lock()->queryMediaPinByID(NS(pin, 1)::VideoStreamOutputPinID) };
			MediaPinRef dataCaptureInputPinRef{ dataCaptureFilterRef.lock()->queryMediaPinByID(NS(pin, 1)::VideoStreamInputPinID) };
//			MediaPinRef videoRendererInputPinRef{ videoRendererFilterRef.lock()->queryMediaPinByID(NS(pin, 1)::VideoStreamInputPinID) };
			if (!livestreamVideoOutputPinRef.expired() && !videoDecoderInputPinRef.expired() &&
				!videoDecoderOutputPinRef.expired() && !dataCaptureInputPinRef.expired())
			{
				livestreamVideoOutputPinRef.lock()->connectPin(videoDecoderInputPinRef);
				videoDecoderOutputPinRef.lock()->connectPin(videoFormatterInputPinRef);
				videoFormatterOutputPinRef.lock()->connectPin(dataCaptureInputPinRef);
			}
		}

		//Live stream capture->Decoder->Renderer(audio)
// 		if (!audioDecoderFilterRef.expired() && !audioRendererFilterRef.expired())
// 		{
// 			MediaPinRef livestreamAudioOutputPinRef{ livestreamCaptureFilterRef.lock()->queryMediaPinByID(NS(pin, 1)::AudioStreamOutputPinID) };
// 			MediaPinRef audioDecoderInputPinRef{ audioDecoderFilterRef.lock()->queryMediaPinByID(NS(pin, 1)::AudioStreamInputPinID) };
// 			MediaPinRef audioDecoderOutputPinRef{ audioDecoderFilterRef.lock()->queryMediaPinByID(NS(pin, 1)::AudioStreamOutputPinID) };
// 			MediaPinRef audioRendererInputPinRef{ audioRendererFilterRef.lock()->queryMediaPinByID(NS(pin, 1)::AudioStreamInputPinID) };
// 			if (!livestreamCaptureFilterRef.expired() && !audioDecoderInputPinRef.expired() &&
// 				!audioDecoderOutputPinRef.expired() && !audioRendererInputPinRef.expired())
// 			{
// 				livestreamAudioOutputPinRef.lock()->connectPin(audioDecoderInputPinRef);
// 				audioDecoderOutputPinRef.lock()->connectPin(audioRendererInputPinRef);
// 			}
// 		}

// 		if (!dataCaptureFilterRef.expired())
// 		{
// 			// Renderer->Capture(video)
// 			MediaPinRef rendererVideoOutputPinRef{ videoRendererFilterRef.lock()->queryMediaPinByID(NS(pin, 1)::VideoStreamOutputPinID) };
// 			MediaPinRef dataCaptureInputPinRef{ dataCaptureFilterRef.lock()->queryMediaPinByID(NS(pin, 1)::VideoStreamInputPinID) };
// 			if (!rendererVideoOutputPinRef.expired() && !dataCaptureInputPinRef.expired())
// 			{
// 				rendererVideoOutputPinRef.lock()->connectPin(dataCaptureInputPinRef);
// 			}
// 
// 			//Renderer->Capture(audio)
// 			MediaPinRef rendererAudioOutputPinRef{ audioRendererFilterRef.lock()->queryMediaPinByID(NS(pin, 1)::AudioStreamInputPinID) };
// 			if (!rendererAudioOutputPinRef.expired() && !dataCaptureInputPinRef.expired())
// 			{
// 				rendererAudioOutputPinRef.lock()->connectPin(dataCaptureInputPinRef);
// 			}
// 		}

		status = ERR_OK;
	}

	return status;
}

int LivestreamMediaGraph::createNewLivestreamCaptureFilter()
{
	int status{ ERR_BAD_ALLOC };
	MediaFilterPtr mediaFilterPtr{ boost::make_shared<LivestreamCaptureFilter>() };

	if (mediaFilterPtr && ERR_OK == mediaFilterPtr->createNewFilter())
	{
		status = addMediaFilter(NS(filter, 1)::AVMediaLivestreamCaptureFilterID, mediaFilterPtr);
	}

	return status;
}

int LivestreamMediaGraph::createNewVideoDecoderFilter()
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

int LivestreamMediaGraph::createNewAudioDecoderFilter()
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

int LivestreamMediaGraph::createNewVideoRendererFilter()
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

int LivestreamMediaGraph::createNewAudioRendererFilter(void)
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

int LivestreamMediaGraph::createNewVideoFormatterFilter(void)
{
	int status{ ERR_BAD_ALLOC };
	MediaFilterPtr mediaFilterPtr{ boost::make_shared<VideoFormatterFilter>() };

	if (mediaFilterPtr && mediaFilterPtr->createNewFilter())
	{
		status = addMediaFilter(NS(filter, 1)::AVMediaVideoFormatterFilterID, mediaFilterPtr);
	}

	return status;
}

int LivestreamMediaGraph::createNewDataCaptureFilter()
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
