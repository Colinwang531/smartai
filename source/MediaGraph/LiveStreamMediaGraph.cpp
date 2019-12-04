#include "boost/bind.hpp"
#include "boost/make_shared.hpp"
#include "error.h"
#include "MediaPin/MediaPin.h"
#include "MediaFilter/Capture/AVCaptureFilter.h"
#include "MediaGraph/LivestreamMediaGraph.h"

namespace framework
{
	namespace multimedia
	{
		LivestreamMediaGraph::LivestreamMediaGraph() : AVPlayMediaGraph()
		{}

		LivestreamMediaGraph::~LivestreamMediaGraph()
		{}

		int LivestreamMediaGraph::createNewGraph()
		{
			return ERR_OK == createNewLivestreamCaptureFilter() ? AVPlayMediaGraph::createNewGraph() : ERR_BAD_ALLOC;
		}

		int LivestreamMediaGraph::buildMediaGraph()
		{
			MediaPinRef inputMediaPinRef, outputMediaPinRef;
			MediaFilterRef livestreamCaptureFilterRef{ queryMediaFilterByID(AVMediaLivestreamCaptureFilterID) },
				videoDecoderFilterRef{ queryMediaFilterByID(AVMediaVideoDecoderFilterID) },
				audioDecoderFilterRef{ queryMediaFilterByID(AVMediaAudioDecoderFilterID) };

			if (!livestreamCaptureFilterRef.expired() && !videoDecoderFilterRef.expired() && !audioDecoderFilterRef.expired())
			{
				outputMediaPinRef = livestreamCaptureFilterRef.lock()->queryMediaPinByID(VideoStreamOutputPinID);
				inputMediaPinRef = videoDecoderFilterRef.lock()->queryMediaPinByID(VideoStreamInputPinID);
				if (!outputMediaPinRef.expired() && !inputMediaPinRef.expired())
				{
					outputMediaPinRef.lock()->connectPin(inputMediaPinRef);
				}
				outputMediaPinRef = livestreamCaptureFilterRef.lock()->queryMediaPinByID(AudioStreamOutputPinID);
				inputMediaPinRef = audioDecoderFilterRef.lock()->queryMediaPinByID(AudioStreamInputPinID);
				if (!outputMediaPinRef.expired() && !inputMediaPinRef.expired())
				{
					outputMediaPinRef.lock()->connectPin(inputMediaPinRef);
				}
			}

			return AVPlayMediaGraph::buildMediaGraph();
		}

		int LivestreamMediaGraph::createNewLivestreamCaptureFilter()
		{
			int status{ ERR_BAD_ALLOC };
			MediaFilterPtr mediaFilterPtr{ boost::make_shared<AVCaptureFilter>() };

			if (mediaFilterPtr && ERR_OK == mediaFilterPtr->createNewFilter())
			{
				status = addMediaFilter(AVMediaLivestreamCaptureFilterID, mediaFilterPtr);
			}

			return status;
		}
	}//namespace multimedia
}//namespace framework
