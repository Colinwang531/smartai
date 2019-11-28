#include "boost/bind.hpp"
#include "boost/make_shared.hpp"
#include "error.h"
#include "MediaPin/MediaPin.h"
#include "MediaFilter/Controller/AVPlayControllerFilter.h"
#include "MediaFilter/Callback/AVDataCallbackFilter.h"
#include "MediaGraph/PlaybackMediaGraph.h"

namespace framework
{
	namespace multimedia
	{
		PlaybackMediaGraph::PlaybackMediaGraph() : MediaGraph()
		{}

		PlaybackMediaGraph::~PlaybackMediaGraph()
		{}

		int PlaybackMediaGraph::createNewGraph()
		{
			int status{ ERR_BAD_ALLOC };

			if (ERR_OK == createNewPlayControllerFilter() && ERR_OK == createNewDataCaptureFilter())
			{
				status = postCreateNewGraph();
			}

			return status;
		}

		int PlaybackMediaGraph::postCreateNewGraph()
		{
			int status{ ERR_BAD_ALLOC };

			if (ERR_OK == createNewDemuxerFilter() && ERR_OK == createNewImageFormatterFilter() &&
				ERR_OK == createNewVideoDecoderFilter() && ERR_OK == createNewVideoRendererFilter() &&
				ERR_OK == createNewAudioDecoderFilter() && ERR_OK == createNewSoundPlayerFilter())
			{
				status = linkMediaFilter();
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

		int PlaybackMediaGraph::createNewDataCaptureFilter()
		{
			int status{ ERR_BAD_ALLOC };
			MediaFilterPtr mediaFilterPtr{ boost::make_shared<AVDataCallbackFilter>() };

			if (mediaFilterPtr && ERR_OK == mediaFilterPtr->createNewFilter(MediaStreamID::MEDIA_STREAM_ID_AV))
			{
				status = addMediaFilter(AVMediaDataCallbackFilterID, mediaFilterPtr);
			}

			return status;
		}

		int PlaybackMediaGraph::linkMediaFilter(void)
		{
			int status{ ERR_BAD_OPERATE };
			MediaPinRef inputMediaPinRef, outputMediaPinRef;
			MediaFilterRef demuxerFilterRef{ queryMediaFilterByID(AVMediaDemuxerFilterID) },
				controllerFilterRef{ queryMediaFilterByID(AVMediaPlaybackControlFilterID) };

			if (!demuxerFilterRef.expired() && !controllerFilterRef.expired())
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

				MediaFilterRef videoDecoderFilterRef{ queryMediaFilterByID(AVMediaVideoDecoderFilterID) },
					videoRendererFilterRef{ queryMediaFilterByID(AVMediaVideoRendererFilterID) },
					imageFormatterFilterRef{ queryMediaFilterByID(AVMediaImageFormatFilterID) },
					dataCallbakFilterRef{ queryMediaFilterByID(AVMediaDataCallbackFilterID) };

				if (!videoDecoderFilterRef.expired() && !videoRendererFilterRef.expired() && 
					!imageFormatterFilterRef.expired() && !dataCallbakFilterRef.expired())
				{
					outputMediaPinRef = controllerFilterRef.lock()->queryMediaPinByID(VideoStreamOutputPinID);
					inputMediaPinRef = videoDecoderFilterRef.lock()->queryMediaPinByID(VideoStreamInputPinID);
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

					status = ERR_OK;
				}

				MediaFilterRef audioDecoderFilterRef{ queryMediaFilterByID(AVMediaAudioDecoderFilterID) },
					soundPlayerFilterRef{ queryMediaFilterByID(AVMediaSoundPlayerFilterID) };

				if (!audioDecoderFilterRef.expired() && !soundPlayerFilterRef.expired() && !dataCallbakFilterRef.expired())
				{
					outputMediaPinRef = controllerFilterRef.lock()->queryMediaPinByID(AudioStreamOutputPinID);
					inputMediaPinRef = audioDecoderFilterRef.lock()->queryMediaPinByID(AudioStreamInputPinID);
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
					outputMediaPinRef = soundPlayerFilterRef.lock()->queryMediaPinByID(AudioStreamOutputPinID);
					inputMediaPinRef = dataCallbakFilterRef.lock()->queryMediaPinByID(AudioStreamInputPinID);
					if (!outputMediaPinRef.expired() && !inputMediaPinRef.expired())
					{
						outputMediaPinRef.lock()->connectPin(inputMediaPinRef);
					}

					status = ERR_OK;
				}
			}

			return status;
		}
	}//namespace multimedia
}//namespace framework
