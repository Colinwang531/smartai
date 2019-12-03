#include "boost/bind.hpp"
#include "boost/make_shared.hpp"
#include "error.h"
#include "MediaPin/MediaPin.h"
#include "MediaFilter/Demuxer/AVDemuxerFilter.h"
#include "MediaFilter/Controller/AVPlayControllerFilter.h"
#include "MediaGraph/PlaybackMediaGraph.h"

namespace framework
{
	namespace multimedia
	{
		PlaybackMediaGraph::PlaybackMediaGraph() : AVPlayMediaGraph()
		{}

		PlaybackMediaGraph::~PlaybackMediaGraph()
		{}

		int PlaybackMediaGraph::createNewGraph()
		{
			return ERR_OK == createNewPlayControllerFilter() && 
				ERR_OK == createNewDemuxerFilter() ? AVPlayMediaGraph::createNewGraph() : ERR_BAD_ALLOC;
		}

		int PlaybackMediaGraph::buildMediaGraph(void)
		{
			MediaPinRef inputMediaPinRef, outputMediaPinRef;
			MediaFilterRef demuxerFilterRef{ queryMediaFilterByID(AVMediaDemuxerFilterID) },
				controllerFilterRef{ queryMediaFilterByID(AVMediaPlaybackControlFilterID) },
				videoDecoderFilterRef{ queryMediaFilterByID(AVMediaVideoDecoderFilterID) }, 
				audioDecoderFilterRef{ queryMediaFilterByID(AVMediaAudioDecoderFilterID) };

			if (!demuxerFilterRef.expired() && !controllerFilterRef.expired() && 
				!videoDecoderFilterRef.expired() && !audioDecoderFilterRef.expired())
			{
				outputMediaPinRef = demuxerFilterRef.lock()->queryMediaPinByID(VideoStreamOutputPinID);
				inputMediaPinRef = controllerFilterRef.lock()->queryMediaPinByID(VideoStreamInputPinID);
				if (!outputMediaPinRef.expired() && !inputMediaPinRef.expired())
				{
					outputMediaPinRef.lock()->connectPin(inputMediaPinRef);
				}
				outputMediaPinRef = demuxerFilterRef.lock()->queryMediaPinByID(AudioStreamOutputPinID);
				inputMediaPinRef = controllerFilterRef.lock()->queryMediaPinByID(AudioStreamInputPinID);
				if (!outputMediaPinRef.expired() && !inputMediaPinRef.expired())
				{
					outputMediaPinRef.lock()->connectPin(inputMediaPinRef);
				}

				outputMediaPinRef = controllerFilterRef.lock()->queryMediaPinByID(VideoStreamOutputPinID);
				inputMediaPinRef = videoDecoderFilterRef.lock()->queryMediaPinByID(VideoStreamInputPinID);
				if (!outputMediaPinRef.expired() && !inputMediaPinRef.expired())
				{
					outputMediaPinRef.lock()->connectPin(inputMediaPinRef);
				}
				outputMediaPinRef = controllerFilterRef.lock()->queryMediaPinByID(AudioStreamOutputPinID);
				inputMediaPinRef = audioDecoderFilterRef.lock()->queryMediaPinByID(AudioStreamInputPinID);
				if (!outputMediaPinRef.expired() && !inputMediaPinRef.expired())
				{
					outputMediaPinRef.lock()->connectPin(inputMediaPinRef);
				}
			}

			return AVPlayMediaGraph::buildMediaGraph();
		}

		int PlaybackMediaGraph::createNewDemuxerFilter(void)
		{
			int status{ ERR_BAD_ALLOC };
			MediaFilterPtr mediaFilterPtr{ boost::make_shared<AVDemuxerFilter>() };

			if (mediaFilterPtr && ERR_OK == mediaFilterPtr->createNewFilter(MediaStreamID::MEDIA_STREAM_ID_AV))
			{
				status = addMediaFilter(AVMediaDemuxerFilterID, mediaFilterPtr);
			}

			return status;
		}

		int PlaybackMediaGraph::createNewPlayControllerFilter(void)
		{
			int status{ ERR_BAD_ALLOC };
			MediaFilterPtr mediaFilterPtr{ boost::make_shared<AVPlayControllerFilter>() };

			if (mediaFilterPtr && ERR_OK == mediaFilterPtr->createNewFilter(MediaStreamID::MEDIA_STREAM_ID_AV))
			{
				status = addMediaFilter(AVMediaPlaybackControlFilterID, mediaFilterPtr);
			}

			return status;
		}
	}//namespace multimedia
}//namespace framework
