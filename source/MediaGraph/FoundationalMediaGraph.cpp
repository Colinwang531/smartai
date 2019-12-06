#include "boost/make_shared.hpp"
#include "error.h"
#include "MediaPin/MediaPin.h"
#include "MediaFilter/Formatter/ImageFormatterFilter.h"
#include "MediaFilter/Renderer/AVRendererFilter.h"
#include "MediaFilter/Callback/AVDataCallbackFilter.h"
#include "MediaGraph/FoundationalMediaGraph.h"

namespace framework
{
	namespace multimedia
	{
		FoundationalMediaGraph::FoundationalMediaGraph() : MediaGraph()
		{}

		FoundationalMediaGraph::~FoundationalMediaGraph()
		{}

		int FoundationalMediaGraph::createNewGraph()
		{
			int status{ ERR_BAD_ALLOC };

			if (ERR_OK == createNewImageFormatterFilter() && ERR_OK == createNewVideoRendererFilter() &&
				ERR_OK == createNewSoundPlayerFilter() && ERR_OK == createNewDataCaptureFilter())
			{
				buildMediaGraph();
				status = ERR_OK;
			}

			return status;
		}

		int FoundationalMediaGraph::createNewImageFormatterFilter()
		{
			int status{ ERR_BAD_ALLOC };
			MediaFilterPtr mediaFilterPtr{ boost::make_shared<ImageFormatterFilter>() };

			if (mediaFilterPtr && ERR_OK == mediaFilterPtr->createNewFilter(MEDIA_PIN_FLAG_VIDEO, MEDIA_PIN_FLAG_VIDEO))
			{
				status = addMediaFilter(AVMediaImageFormatFilterID, mediaFilterPtr);
			}

			return status;
		}

		int FoundationalMediaGraph::createNewVideoRendererFilter()
		{
			int status{ ERR_BAD_ALLOC };
			MediaFilterPtr mediaFilterPtr{ boost::make_shared<AVRendererFilter>() };

			if (mediaFilterPtr && ERR_OK == mediaFilterPtr->createNewFilter(MEDIA_PIN_FLAG_VIDEO, MEDIA_PIN_FLAG_VIDEO))
			{
				status = addMediaFilter(AVMediaVideoRendererFilterID, mediaFilterPtr);
			}

			return status;
		}

		int FoundationalMediaGraph::createNewSoundPlayerFilter()
		{
			int status{ ERR_BAD_ALLOC };
			MediaFilterPtr mediaFilterPtr{ boost::make_shared<AVRendererFilter>() };

			if (mediaFilterPtr && ERR_OK == mediaFilterPtr->createNewFilter(MEDIA_PIN_FLAG_AUDIO, MEDIA_PIN_FLAG_AUDIO))
			{
				status = addMediaFilter(AVMediaSoundPlayerFilterID, mediaFilterPtr);
			}

			return status;
		}

		int FoundationalMediaGraph::createNewDataCaptureFilter()
		{
			int status{ ERR_BAD_ALLOC };
			MediaFilterPtr mediaFilterPtr{ boost::make_shared<AVDataCallbackFilter>() };

			if (mediaFilterPtr && ERR_OK == mediaFilterPtr->createNewFilter(MEDIA_PIN_FLAG_VIDEO | MEDIA_PIN_FLAG_AUDIO, MEDIA_PIN_FLAG_NONE))
			{
				status = addMediaFilter(AVMediaDataCallbackFilterID, mediaFilterPtr);
			}

			return status;
		}

		void FoundationalMediaGraph::buildMediaGraph()
		{
			MediaPinRef inputMediaPinRef, outputMediaPinRef;
			MediaFilterRef imageFormatterFilterRef{ queryMediaFilterByID(AVMediaImageFormatFilterID) },
				videoRendererFilterRef{ queryMediaFilterByID(AVMediaVideoRendererFilterID) },
				soundPlayerFilterRef{ queryMediaFilterByID(AVMediaSoundPlayerFilterID) },
				dataCallbakFilterRef{ queryMediaFilterByID(AVMediaDataCallbackFilterID) };

			if (!imageFormatterFilterRef.expired() && !videoRendererFilterRef.expired() && !dataCallbakFilterRef.expired())
			{
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

			if (!soundPlayerFilterRef.expired() && !dataCallbakFilterRef.expired())
			{
				outputMediaPinRef = soundPlayerFilterRef.lock()->queryMediaPinByID(AudioStreamOutputPinID);
				inputMediaPinRef = dataCallbakFilterRef.lock()->queryMediaPinByID(AudioStreamInputPinID);
				if (!outputMediaPinRef.expired() && !inputMediaPinRef.expired())
				{
					outputMediaPinRef.lock()->connectPin(inputMediaPinRef);
				}
			}
		}
	}//namespace multimedia
}//namespace framework
