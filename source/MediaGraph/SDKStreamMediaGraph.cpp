#include "boost/make_shared.hpp"
#include "error.h"
#include "MediaPin/MediaPin.h"
#include "MediaFilter/Capture/AVCaptureFilter.h"
#include "MediaFilter/Decoder/AVDecoderFilter.h"
#include "MediaGraph/FoundationalMediaGraph.h"
#include "MediaGraph/SDKStreamMediaGraph.h"

namespace framework
{
	namespace multimedia
	{
		SDKStreamMediaGraph::SDKStreamMediaGraph() : MediaGraph()
		{}

		SDKStreamMediaGraph::~SDKStreamMediaGraph()
		{}

		int SDKStreamMediaGraph::createNewGraph()
		{
			int status{ ERR_BAD_ALLOC };
			MediaGraphPtr mediaGraphPtr{ boost::make_shared<FoundationalMediaGraph>() };

			if (mediaGraphPtr && ERR_OK == mediaGraphPtr->createNewGraph())
			{
				if (ERR_OK == createNewDataCaptureFilter() && ERR_OK == createNewSDKDecoderFilter())
				{
					buildMediaGraph();
					status = ERR_OK;
				}

				foundationalMediaGraphPtr.swap(mediaGraphPtr);
			}

			return status;
		}

		int SDKStreamMediaGraph::createNewDataCaptureFilter()
		{
			int status{ ERR_BAD_ALLOC };
			MediaFilterPtr mediaFilterPtr{ boost::make_shared<AVCaptureFilter>() };

			if (mediaFilterPtr && ERR_OK == mediaFilterPtr->createNewFilter(MEDIA_PIN_FLAG_NONE, MEDIA_PIN_FLAG_VIDEO))
			{
				status = addMediaFilter(AVMediaDataCaptureFilterID, mediaFilterPtr);
			}

			return status;
		}

		int SDKStreamMediaGraph::createNewSDKDecoderFilter()
		{
			int status{ ERR_BAD_ALLOC };
			MediaFilterPtr mediaFilterPtr{ boost::make_shared<AVDecoderFilter>() };

			if (mediaFilterPtr && ERR_OK == mediaFilterPtr->createNewFilter(MEDIA_PIN_FLAG_VIDEO, MEDIA_PIN_FLAG_VIDEO | MEDIA_PIN_FLAG_AUDIO))
			{
				status = addMediaFilter(AVMediaSDKDecoderFilterID, mediaFilterPtr);
			}

			return status;
		}

		void SDKStreamMediaGraph::buildMediaGraph()
		{
			MediaPinRef inputMediaPinRef, outputMediaPinRef;
			MediaFilterRef dataCaptureFilterRef{ queryMediaFilterByID(AVMediaDataCaptureFilterID) },
				sdkDecoderFilterRef{ queryMediaFilterByID(AVMediaSDKDecoderFilterID) },
				imageFormatterFilterRef{ queryMediaFilterByID(AVMediaImageFormatFilterID) },
				soundPlayerFilterRef{ queryMediaFilterByID(AVMediaSoundPlayerFilterID) };

			if (!dataCaptureFilterRef.expired() && !sdkDecoderFilterRef.expired() && 
				!imageFormatterFilterRef.expired() && !soundPlayerFilterRef.expired())
			{
				outputMediaPinRef = dataCaptureFilterRef.lock()->queryMediaPinByID(VideoStreamOutputPinID);
				inputMediaPinRef = sdkDecoderFilterRef.lock()->queryMediaPinByID(VideoStreamInputPinID);
				if (!outputMediaPinRef.expired() && !inputMediaPinRef.expired())
				{
					outputMediaPinRef.lock()->connectPin(inputMediaPinRef);
				}
				outputMediaPinRef = sdkDecoderFilterRef.lock()->queryMediaPinByID(VideoStreamOutputPinID);
				inputMediaPinRef = imageFormatterFilterRef.lock()->queryMediaPinByID(VideoStreamInputPinID);
				if (!outputMediaPinRef.expired() && !inputMediaPinRef.expired())
				{
					outputMediaPinRef.lock()->connectPin(inputMediaPinRef);
				}
				outputMediaPinRef = sdkDecoderFilterRef.lock()->queryMediaPinByID(AudioStreamOutputPinID);
				inputMediaPinRef = soundPlayerFilterRef.lock()->queryMediaPinByID(AudioStreamInputPinID);
				if (!outputMediaPinRef.expired() && !inputMediaPinRef.expired())
				{
					outputMediaPinRef.lock()->connectPin(inputMediaPinRef);
				}
			}
		}
	}//namespace multimedia
}//namespace framework
