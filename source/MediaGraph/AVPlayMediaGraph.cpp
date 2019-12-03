#include "boost/bind.hpp"
#include "boost/make_shared.hpp"
#include "error.h"
#include "MediaPin/MediaPin.h"
#include "MediaFilter/Formatter/ImageFormatterFilter.h"
#include "MediaFilter/Decoder/AVDecoderFilter.h"
#include "MediaFilter/Renderer/AVRendererFilter.h"
#include "MediaFilter/Callback/AVDataCallbackFilter.h"
#include "MediaGraph/AVPlayMediaGraph.h"

namespace framework
{
	namespace multimedia
	{
		AVPlayMediaGraph::AVPlayMediaGraph() : MediaGraph()
		{}

		AVPlayMediaGraph::~AVPlayMediaGraph()
		{}

		int AVPlayMediaGraph::createNewGraph()
		{
			int status{ ERR_BAD_ALLOC };

			if (ERR_OK == createNewImageFormatterFilter() &&
				ERR_OK == createNewVideoDecoderFilter() &&
				ERR_OK == createNewAudioDecoderFilter() &&
				ERR_OK == createNewVideoRendererFilter() &&
				ERR_OK == createNewSoundPlayerFilter() &&
				ERR_OK == createNewDataCaptureFilter())
			{
				status = buildMediaGraph();
			}

			return status;
		}

		int AVPlayMediaGraph::buildMediaGraph(void)
		{
			MediaPinRef inputMediaPinRef, outputMediaPinRef;
			MediaFilterRef videoDecoderFilterRef{ queryMediaFilterByID(AVMediaVideoDecoderFilterID) },
				videoRendererFilterRef{ queryMediaFilterByID(AVMediaVideoRendererFilterID) },
				audioDecoderFilterRef{ queryMediaFilterByID(AVMediaAudioDecoderFilterID) },
				soundPlayerFilterRef{ queryMediaFilterByID(AVMediaSoundPlayerFilterID) },
				imageFormatterFilterRef{ queryMediaFilterByID(AVMediaImageFormatFilterID) },
				dataCallbakFilterRef{ queryMediaFilterByID(AVMediaDataCallbackFilterID) };

			if (!videoDecoderFilterRef.expired() && !videoRendererFilterRef.expired() &&
				!imageFormatterFilterRef.expired() && !dataCallbakFilterRef.expired())
			{
				outputMediaPinRef = videoDecoderFilterRef.lock()->queryMediaPinByID(VideoStreamOutputPinID);
				inputMediaPinRef = imageFormatterFilterRef.lock()->queryMediaPinByID(VideoStreamInputPinID);
				if (!outputMediaPinRef.expired() && !inputMediaPinRef.expired())
				{
					outputMediaPinRef.lock()->connectPin(inputMediaPinRef);
				}
				outputMediaPinRef = imageFormatterFilterRef.lock()->queryMediaPinByID(VideoStreamOutputPinID);
				inputMediaPinRef = videoRendererFilterRef.lock()->queryMediaPinByID(VideoStreamInputPinID);
				if (!outputMediaPinRef.expired() && !inputMediaPinRef.expired())
				{
					outputMediaPinRef.lock()->connectPin(inputMediaPinRef);
				}
				outputMediaPinRef = videoRendererFilterRef.lock()->queryMediaPinByID(VideoStreamOutputPinID);
				inputMediaPinRef = dataCallbakFilterRef.lock()->queryMediaPinByID(VideoStreamInputPinID);
				if (!outputMediaPinRef.expired() && !inputMediaPinRef.expired())
				{
					outputMediaPinRef.lock()->connectPin(inputMediaPinRef);
				}
			}

			if (!audioDecoderFilterRef.expired() && !soundPlayerFilterRef.expired() && !dataCallbakFilterRef.expired())
			{
				outputMediaPinRef = audioDecoderFilterRef.lock()->queryMediaPinByID(AudioStreamOutputPinID);
				inputMediaPinRef = soundPlayerFilterRef.lock()->queryMediaPinByID(AudioStreamInputPinID);
				if (!outputMediaPinRef.expired() && !inputMediaPinRef.expired())
				{
					outputMediaPinRef.lock()->connectPin(inputMediaPinRef);
				}
				outputMediaPinRef = soundPlayerFilterRef.lock()->queryMediaPinByID(AudioStreamOutputPinID);
				inputMediaPinRef = dataCallbakFilterRef.lock()->queryMediaPinByID(AudioStreamInputPinID);
				if (!outputMediaPinRef.expired() && !inputMediaPinRef.expired())
				{
					outputMediaPinRef.lock()->connectPin(inputMediaPinRef);
				}
			}

			return ERR_OK;
		}

		int AVPlayMediaGraph::createNewImageFormatterFilter(void)
		{
			int status{ ERR_BAD_ALLOC };
			MediaFilterPtr mediaFilterPtr{ boost::make_shared<ImageFormatterFilter>() };

			if (mediaFilterPtr && ERR_OK == mediaFilterPtr->createNewFilter(MediaStreamID::MEDIA_STREAM_ID_VIDEO))
			{
				status = addMediaFilter(AVMediaImageFormatFilterID, mediaFilterPtr);
			}

			return status;
		}

		int AVPlayMediaGraph::createNewVideoDecoderFilter(void)
		{
			int status{ ERR_BAD_ALLOC };
			MediaFilterPtr mediaFilterPtr{ boost::make_shared<AVDecoderFilter>() };

			if (mediaFilterPtr && ERR_OK == mediaFilterPtr->createNewFilter(MediaStreamID::MEDIA_STREAM_ID_VIDEO))
			{
				status = addMediaFilter(AVMediaVideoDecoderFilterID, mediaFilterPtr);
			}

			return status;
		}

		int AVPlayMediaGraph::createNewAudioDecoderFilter(void)
		{
			int status{ ERR_BAD_ALLOC };
			MediaFilterPtr mediaFilterPtr{ boost::make_shared<AVDecoderFilter>() };

			if (mediaFilterPtr && ERR_OK == mediaFilterPtr->createNewFilter(MediaStreamID::MEDIA_STREAM_ID_AUDIO))
			{
				status = addMediaFilter(AVMediaAudioDecoderFilterID, mediaFilterPtr);
			}

			return status;
		}

		int AVPlayMediaGraph::createNewVideoRendererFilter(void)
		{
			int status{ ERR_BAD_ALLOC };
			MediaFilterPtr mediaFilterPtr{ boost::make_shared<AVRendererFilter>() };

			if (mediaFilterPtr && ERR_OK == mediaFilterPtr->createNewFilter(MediaStreamID::MEDIA_STREAM_ID_VIDEO))
			{
				status = addMediaFilter(AVMediaVideoRendererFilterID, mediaFilterPtr);
			}

			return status;
		}

		int AVPlayMediaGraph::createNewSoundPlayerFilter(void)
		{
			int status{ ERR_BAD_ALLOC };
			MediaFilterPtr mediaFilterPtr{ boost::make_shared<AVRendererFilter>() };

			if (mediaFilterPtr && ERR_OK == mediaFilterPtr->createNewFilter(MediaStreamID::MEDIA_STREAM_ID_AUDIO))
			{
				status = addMediaFilter(AVMediaSoundPlayerFilterID, mediaFilterPtr);
			}

			return status;
		}

		int AVPlayMediaGraph::createNewDataCaptureFilter()
		{
			int status{ ERR_BAD_ALLOC };
			MediaFilterPtr mediaFilterPtr{ boost::make_shared<AVDataCallbackFilter>() };

			if (mediaFilterPtr && ERR_OK == mediaFilterPtr->createNewFilter(MediaStreamID::MEDIA_STREAM_ID_AV))
			{
				status = addMediaFilter(AVMediaDataCallbackFilterID, mediaFilterPtr);
			}

			return status;
		}
	}//namespace multimedia
}//namespace framework
