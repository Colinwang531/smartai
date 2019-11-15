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

int PlaybackMediaGraph::createNewGraph(void* hwnd /* = NULL */, void* callback /* = NULL */)
{
	displayHwnd = hwnd;
	userCallback = callback;
	// Default creating demuxer filter by framework.
	createNewDemuxerFilter();
	createNewControllerFilter();
	createNewCaptureFilter(callback);

	return ERR_OK;
}

int PlaybackMediaGraph::destroyGraph(void)
{
	return 0;
}

int PlaybackMediaGraph::createNewVideoDecoderFilter(
	const MediaDataSubID videoSubID /* = MediaDataSubID::MEDIA_DATA_SUB_ID_NONE */)
{
	int status{ ERR_BAD_ALLOC };
	MediaFilterPtr mediaFilterPtr{ boost::make_shared<AVDecoderFilter>(videoSubID) };

	if (mediaFilterPtr && ERR_OK == mediaFilterPtr->createNewFilter())
	{
		status = addMediaFilter(NS(filter, 1)::AVMediaVideoDecoderFilterID, mediaFilterPtr);

		if (ERR_OK == status)
		{
			MediaFilterRef mediaFilterRef{ queryMediaFilterByID(NS(filter, 1)::AVMediaControllerFilterID) };
			if (!mediaFilterRef.expired())
			{
				MediaPinRef controllerVideoOutputPinRef{ mediaFilterRef.lock()->queryMediaPinByID(NS(pin, 1)::VideoStreamOutputPinID) };
				MediaPinRef decoderVideoInputPinRef{ mediaFilterPtr->queryMediaPinByID(NS(pin, 1)::VideoStreamInputPinID) };
				if (!controllerVideoOutputPinRef.expired() && !decoderVideoInputPinRef.expired())
				{
					controllerVideoOutputPinRef.lock()->connectPin(decoderVideoInputPinRef);
				}
			}
		}
	}

	return status;
}

int PlaybackMediaGraph::createNewAudioDecoderFilter(
	const MediaDataSubID audioSubID /* = MediaDataSubID::MEDIA_DATA_SUB_ID_NONE */)
{
	int status{ ERR_BAD_ALLOC };
	MediaFilterPtr mediaFilterPtr{ boost::make_shared<AVDecoderFilter>(audioSubID) };

	if (mediaFilterPtr && ERR_OK == mediaFilterPtr->createNewFilter())
	{
		status = addMediaFilter(NS(filter, 1)::AVMediaAudioDecoderFilterID, mediaFilterPtr);

		if (ERR_OK == status)
		{
			MediaFilterRef mediaFilterRef{ queryMediaFilterByID(NS(filter, 1)::AVMediaControllerFilterID) };
			if (!mediaFilterRef.expired())
			{
				MediaPinRef controllerAudioOutputPinRef{ mediaFilterRef.lock()->queryMediaPinByID(NS(pin, 1)::AudioStreamOutputPinID) };
				MediaPinRef decoderAudioInputPinRef{ mediaFilterPtr->queryMediaPinByID(NS(pin, 1)::AudioStreamInputPinID) };
				if (!controllerAudioOutputPinRef.expired() && !decoderAudioInputPinRef.expired())
				{
					controllerAudioOutputPinRef.lock()->connectPin(decoderAudioInputPinRef);
				}
			}
		}
	}

	return status;
}

int PlaybackMediaGraph::createNewVideoRendererFilter()
{
	int status{ ERR_BAD_ALLOC };
	MediaFilterPtr mediaFilterPtr{ boost::make_shared<AVRendererFilter>(reinterpret_cast<HWND>(displayHwnd)) };

	if (mediaFilterPtr && mediaFilterPtr->createNewFilter())
	{
		status = addMediaFilter(NS(filter, 1)::AVMediaVideoRendererFilterID, mediaFilterPtr);

		if (ERR_OK == status)
		{
			MediaFilterRef decoderMediaFilterRef{ queryMediaFilterByID(NS(filter, 1)::AVMediaVideoDecoderFilterID) };
			MediaFilterRef captureMediaFilterRef{ queryMediaFilterByID(NS(filter, 1)::AVMediaDataCaptureFilterID) };

			if (!decoderMediaFilterRef.expired() && !captureMediaFilterRef.expired())
			{
				MediaPinRef decoderVideoOutputPinRef{ decoderMediaFilterRef.lock()->queryMediaPinByID(NS(pin, 1)::VideoStreamOutputPinID) };
				MediaPinRef rendererVideoInputPinRef{ mediaFilterPtr->queryMediaPinByID(NS(pin, 1)::VideoStreamInputPinID) };
				if (!decoderVideoOutputPinRef.expired() && !rendererVideoInputPinRef.expired())
				{
					decoderVideoOutputPinRef.lock()->connectPin(rendererVideoInputPinRef);
				}

				MediaPinRef rendererVideoOutputPinRef{ mediaFilterPtr->queryMediaPinByID(NS(pin, 1)::VideoStreamOutputPinID) };
				MediaPinRef captureMediaInputPinRef{ captureMediaFilterRef.lock()->queryMediaPinByID(NS(pin, 1)::MediaStreamInputPinID) };
				if (!rendererVideoOutputPinRef.expired() && !captureMediaInputPinRef.expired())
				{
					rendererVideoOutputPinRef.lock()->connectPin(captureMediaInputPinRef);
				}
			}
		}
	}

	return status;
}

int PlaybackMediaGraph::createNewAudioRendererFilter(void)
{
	int status{ ERR_BAD_ALLOC };
	MediaFilterPtr mediaFilterPtr{ boost::make_shared<AVRendererFilter>() };

	if (mediaFilterPtr && mediaFilterPtr->createNewFilter())
	{
		status = addMediaFilter(NS(filter, 1)::AVMediaAudioRendererFilterID, mediaFilterPtr);

		if (ERR_OK == status)
		{
			MediaFilterRef decoderMediaFilterRef{ queryMediaFilterByID(NS(filter, 1)::AVMediaAudioDecoderFilterID) };
			MediaFilterRef captureMediaFilterRef{ queryMediaFilterByID(NS(filter, 1)::AVMediaDataCaptureFilterID) };

			if (!decoderMediaFilterRef.expired() && !captureMediaFilterRef.expired())
			{
				MediaPinRef decoderAudioOutputPinRef{ decoderMediaFilterRef.lock()->queryMediaPinByID(NS(pin, 1)::AudioStreamOutputPinID) };
				MediaPinRef rendererAudioInputPinRef{ mediaFilterPtr->queryMediaPinByID(NS(pin, 1)::AudioStreamInputPinID) };
				if (!decoderAudioOutputPinRef.expired() && !rendererAudioInputPinRef.expired())
				{
					decoderAudioOutputPinRef.lock()->connectPin(rendererAudioInputPinRef);
				}

				MediaPinRef rendererAudioOutputPinRef{ mediaFilterPtr->queryMediaPinByID(NS(pin, 1)::AudioStreamOutputPinID) };
				MediaPinRef captureMediaInputPinRef{ captureMediaFilterRef.lock()->queryMediaPinByID(NS(pin, 1)::MediaStreamInputPinID) };
				if (!rendererAudioOutputPinRef.expired() && !captureMediaInputPinRef.expired())
				{
					rendererAudioOutputPinRef.lock()->connectPin(captureMediaInputPinRef);
				}
			}
		}
	}

	return status;
}

int PlaybackMediaGraph::createNewDemuxerFilter()
{
	int status{ ERR_BAD_ALLOC };
	MediaFilterPtr mediaFilterPtr{ boost::make_shared<AVDemuxerFilter>() };

	if (mediaFilterPtr && ERR_OK == mediaFilterPtr->createNewFilter())
	{
		MediaModelRef mediaModelRef{ mediaFilterPtr->getMediaModel() };
		if (!mediaModelRef.expired())
		{
			mediaModelRef.lock()->setNotifyMediaStreamIDCallback(
				boost::bind(&PlaybackMediaGraph::notifyMediaDataSubIDCallback, this, _1, _2));
		}
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

		if (ERR_OK == status)
		{
			MediaFilterRef mediaFilterRef{ queryMediaFilterByID(NS(filter, 1)::AVMediaDemuxerFilterID) };
			if (!mediaFilterRef.expired())
			{
				MediaPinRef demuxerVideoOutputPinRef{ mediaFilterRef.lock()->queryMediaPinByID(NS(pin, 1)::VideoStreamOutputPinID) };
				MediaPinRef controllerVideoInputPinRef{ mediaFilterPtr->queryMediaPinByID(NS(pin, 1)::VideoStreamInputPinID) };
				if (!demuxerVideoOutputPinRef.expired() && !controllerVideoInputPinRef.expired())
				{
					demuxerVideoOutputPinRef.lock()->connectPin(controllerVideoInputPinRef);
				}

				MediaPinRef demuxerAudioOutputPinRef{ mediaFilterRef.lock()->queryMediaPinByID(NS(pin, 1)::AudioStreamOutputPinID) };
				MediaPinRef controllerAudioInputPinRef{ mediaFilterPtr->queryMediaPinByID(NS(pin, 1)::AudioStreamInputPinID) };
				if (!demuxerAudioOutputPinRef.expired() && !controllerAudioInputPinRef.expired())
				{
					demuxerAudioOutputPinRef.lock()->connectPin(controllerAudioInputPinRef);
				}
			}
		}
	}

	return status;
}

int PlaybackMediaGraph::createNewCaptureFilter(void* callback /* = NULL */)
{
	int status{ ERR_BAD_ALLOC };
	MediaFilterPtr mediaFilterPtr{ boost::make_shared<AVCaptureFilter>(callback) };

	if (mediaFilterPtr && mediaFilterPtr->createNewFilter())
	{
		status = addMediaFilter(NS(filter, 1)::AVMediaDataCaptureFilterID, mediaFilterPtr);
	}

	return status;
}

int PlaybackMediaGraph::notifyMediaDataSubIDCallback(
	const MediaDataSubID videoSubID /* = MediaDataSubID::MEDIA_DATA_SUB_ID_NONE */, 
	const MediaDataSubID audioSubID /* = MediaDataSubID::MEDIA_DATA_SUB_ID_NONE */)
{
	int status{ ERR_NOT_SUPPORT };

	if (MediaDataSubID::MEDIA_DATA_SUB_ID_H264 == videoSubID || MediaDataSubID::MEDIA_DATA_SUB_ID_H265 == videoSubID)
	{
		status = createNewVideoDecoderFilter(videoSubID);
		if (ERR_OK == status)
		{
			status = createNewVideoRendererFilter();
		}
	}
	
	if (MediaDataSubID::MEDIA_DATA_SUB_ID_AAC == audioSubID)
	{
		status = createNewAudioDecoderFilter(audioSubID);
		if (ERR_OK == status)
		{
			status = createNewAudioRendererFilter();
		}
	}
	else if(MediaDataSubID::MEDIA_DATA_SUB_ID_G722 == audioSubID)
	{ }

	return status;
}

NS_END
