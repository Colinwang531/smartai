#include "boost/make_shared.hpp"
#include "error.h"
#include "MediaPin/MediaPin.h"
#include "MediaFilter/Capture/AVCaptureFilter.h"
#include "MediaFilter/Controller/AVPlayControllerFilter.h"
#include "MediaFilter/Decoder/AVDecoderFilter.h"
#include "MediaGraph/FoundationalMediaGraph.h"
#include "MediaGraph/StandardStreamMediaGraph.h"

namespace framework
{
	namespace multimedia
	{
		StandardStreamMediaGraph::StandardStreamMediaGraph() : MediaGraph()
		{}

		StandardStreamMediaGraph::~StandardStreamMediaGraph()
		{}

		int StandardStreamMediaGraph::createNewGraph()
		{
			int status{ ERR_BAD_ALLOC };
			MediaGraphPtr mediaGraphPtr{ boost::make_shared<FoundationalMediaGraph>() };

			if (mediaGraphPtr && ERR_OK == mediaGraphPtr->createNewGraph())
			{
				if (ERR_OK == createNewDataCaptureFilter() && ERR_OK == createNewPlayControllerFilter() &&
					ERR_OK == createNewVideoDecoderFilter() && ERR_OK == createNewAudioDecoderFilter())
				{
					buildMediaGraph();
					status = ERR_OK;
				}

				foundationalMediaGraphPtr.swap(mediaGraphPtr);
			}

			return status;
		}

		int StandardStreamMediaGraph::createNewDataCaptureFilter()
		{
			int status{ ERR_BAD_ALLOC };
			MediaFilterPtr mediaFilterPtr{ boost::make_shared<AVCaptureFilter>() };

			if (mediaFilterPtr && ERR_OK == mediaFilterPtr->createNewFilter(MEDIA_PIN_FLAG_NONE, MEDIA_PIN_FLAG_VIDEO | MEDIA_PIN_FLAG_AUDIO))
			{
				status = addMediaFilter(AVMediaDataCaptureFilterID, mediaFilterPtr);
			}

			return status;
		}

		int StandardStreamMediaGraph::createNewPlayControllerFilter()
		{
			int status{ ERR_BAD_ALLOC };
			MediaFilterPtr mediaFilterPtr{ boost::make_shared<AVPlayControllerFilter>() };

			if (mediaFilterPtr && ERR_OK == mediaFilterPtr->createNewFilter(MEDIA_PIN_FLAG_VIDEO | MEDIA_PIN_FLAG_AUDIO, MEDIA_PIN_FLAG_VIDEO | MEDIA_PIN_FLAG_AUDIO))
			{
				status = addMediaFilter(AVMediaPlayControlFilterID, mediaFilterPtr);
			}

			return status;
		}

		int StandardStreamMediaGraph::createNewVideoDecoderFilter()
		{
			int status{ ERR_BAD_ALLOC };
			MediaFilterPtr mediaFilterPtr{ boost::make_shared<AVDecoderFilter>() };

			if (mediaFilterPtr && ERR_OK == mediaFilterPtr->createNewFilter(MEDIA_PIN_FLAG_VIDEO, MEDIA_PIN_FLAG_VIDEO))
			{
				status = addMediaFilter(AVMediaVideoDecoderFilterID, mediaFilterPtr);
			}

			return status;
		}

		int StandardStreamMediaGraph::createNewAudioDecoderFilter()
		{
			int status{ ERR_BAD_ALLOC };
			MediaFilterPtr mediaFilterPtr{ boost::make_shared<AVDecoderFilter>() };

			if (mediaFilterPtr && ERR_OK == mediaFilterPtr->createNewFilter(MEDIA_PIN_FLAG_AUDIO, MEDIA_PIN_FLAG_AUDIO))
			{
				status = addMediaFilter(AVMediaAudioDecoderFilterID, mediaFilterPtr);
			}

			return status;
		}

		void StandardStreamMediaGraph::buildMediaGraph()
		{
			MediaPinRef inputMediaPinRef, outputMediaPinRef;
			MediaFilterRef dataCaptureFilterRef{ queryMediaFilterByID(AVMediaDataCaptureFilterID) },
				playControllerFilterRef{ queryMediaFilterByID(AVMediaPlayControlFilterID) },
				videoDecoderFilterRef{ queryMediaFilterByID(AVMediaVideoDecoderFilterID) },
				audioDecoderFilterRef{ queryMediaFilterByID(AVMediaAudioDecoderFilterID) },
				imageFormatterFilterRef{ queryMediaFilterByID(AVMediaImageFormatFilterID) },
				soundPlayerFilterRef{ queryMediaFilterByID(AVMediaSoundPlayerFilterID) };

			if (!dataCaptureFilterRef.expired() && !playControllerFilterRef.expired() && !videoDecoderFilterRef.expired() &&
				!audioDecoderFilterRef.expired() && !imageFormatterFilterRef.expired() && !soundPlayerFilterRef.expired())
			{
				outputMediaPinRef = dataCaptureFilterRef.lock()->queryMediaPinByID(VideoStreamOutputPinID);
				inputMediaPinRef = playControllerFilterRef.lock()->queryMediaPinByID(VideoStreamInputPinID);
				if (!outputMediaPinRef.expired() && !inputMediaPinRef.expired())
				{
					outputMediaPinRef.lock()->connectPin(inputMediaPinRef);
				}
				outputMediaPinRef = dataCaptureFilterRef.lock()->queryMediaPinByID(AudioStreamOutputPinID);
				inputMediaPinRef = playControllerFilterRef.lock()->queryMediaPinByID(AudioStreamInputPinID);
				if (!outputMediaPinRef.expired() && !inputMediaPinRef.expired())
				{
					outputMediaPinRef.lock()->connectPin(inputMediaPinRef);
				}

				outputMediaPinRef = playControllerFilterRef.lock()->queryMediaPinByID(VideoStreamOutputPinID);
				inputMediaPinRef = videoDecoderFilterRef.lock()->queryMediaPinByID(VideoStreamInputPinID);
				if (!outputMediaPinRef.expired() && !inputMediaPinRef.expired())
				{
					outputMediaPinRef.lock()->connectPin(inputMediaPinRef);
				}
				outputMediaPinRef = playControllerFilterRef.lock()->queryMediaPinByID(AudioStreamOutputPinID);
				inputMediaPinRef = audioDecoderFilterRef.lock()->queryMediaPinByID(AudioStreamInputPinID);
				if (!outputMediaPinRef.expired() && !inputMediaPinRef.expired())
				{
					outputMediaPinRef.lock()->connectPin(inputMediaPinRef);
				}

				outputMediaPinRef = videoDecoderFilterRef.lock()->queryMediaPinByID(VideoStreamOutputPinID);
				inputMediaPinRef = imageFormatterFilterRef.lock()->queryMediaPinByID(VideoStreamInputPinID);
				if (!outputMediaPinRef.expired() && !inputMediaPinRef.expired())
				{
					outputMediaPinRef.lock()->connectPin(inputMediaPinRef);
				}
				outputMediaPinRef = audioDecoderFilterRef.lock()->queryMediaPinByID(AudioStreamOutputPinID);
				inputMediaPinRef = soundPlayerFilterRef.lock()->queryMediaPinByID(AudioStreamInputPinID);
				if (!outputMediaPinRef.expired() && !inputMediaPinRef.expired())
				{
					outputMediaPinRef.lock()->connectPin(inputMediaPinRef);
				}
			}
		}
	}//namespace multimedia
}//namespace framework
