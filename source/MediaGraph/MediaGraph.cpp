#include "boost/make_shared.hpp"
#include "error.h"
#include "Url/Url.h"
using URL = framework::wrapper::URL;
#include "MediaPin/MediaPin.h"
#include "MediaFilter/Capture/AVCaptureFilter.h"
#include "MediaFilter/Controller/AVPlayControllerFilter.h"
#include "MediaFilter/Decoder/AVDecoderFilter.h"
#include "MediaFilter/Formatter/ImageFormatterFilter.h"
#include "MediaFilter/Renderer/AVRendererFilter.h"
#include "MediaFilter/Callback/AVDataCallbackFilter.h"
#include "MediaGraph/MediaGraph.h"

namespace framework
{
	namespace multimedia
	{
		MediaGraph::MediaGraph()
		{}

		MediaGraph::~MediaGraph()
		{
			closeStream();
		}

		int MediaGraph::openStream(const std::string& streamURL)
		{
			int status{ streamURL.empty() ? ERR_INVALID_PARAM : ERR_OK };

			if (ERR_OK == status &&
				ERR_OK == createNewCaptureFilter(streamURL) &&
				ERR_OK == createNewControllerFilter(streamURL) &&
				ERR_OK == createNewDecoderFilter(streamURL) &&
				ERR_OK == createNewFormatterFilter() &&
				ERR_OK == createNewRendererFilter(streamURL) &&
				ERR_OK == createNewCallbackFilter() &&
				ERR_OK == buildMediaGraph())
			{
				MediaFilterRef captureFilterRef{ queryMediaFilterByID(AVMediaDataCaptureFilterID) };
				if (!captureFilterRef.expired())
				{
					boost::shared_ptr<AVCaptureFilter> captureFilterPtr{ 
						boost::dynamic_pointer_cast<AVCaptureFilter>(captureFilterRef.lock()) };
					if (captureFilterPtr)
					{
						status = captureFilterPtr->startCapture(streamURL);
					}
				}
			}

			return status;
		}

		int MediaGraph::closeStream(void)
		{
			if (0 < mediaFilterGroup.size())
			{
				mediaFilterGroup.clear();
			}

			return ERR_OK;
		}

		MediaFilterRef MediaGraph::queryMediaFilterByID(const std::string& filterID)
		{
			return mediaFilterGroup.at(filterID);
		}

		int MediaGraph::createNewCaptureFilter(const std::string& streamURL)
		{
			int status{ ERR_BAD_ALLOC };
			MediaFilterPtr mediaFilterPtr{ boost::make_shared<AVCaptureFilter>() };
			if (mediaFilterPtr && ERR_OK == mediaFilterPtr->createNewFilter(streamURL))
			{
				mediaFilterGroup.insert(AVMediaDataCaptureFilterID, mediaFilterPtr);
				status = ERR_OK;
			}

			return status;
		}

		int MediaGraph::createNewControllerFilter(const std::string& streamURL)
		{
			int status{ ERR_BAD_ALLOC };
			const framework::wrapper::URLProtocol protocol{ URL(streamURL).getProtocol() };

			if (framework::wrapper::URLProtocol::URL_PROTOCOL_PLAYBACK == protocol)
			{
				MediaFilterPtr mediaFilterPtr{ boost::make_shared<AVPlayControllerFilter>() };
				if (mediaFilterPtr && ERR_OK == mediaFilterPtr->createNewFilter(streamURL))
				{
					mediaFilterGroup.insert(AVMediaPlayControlFilterID, mediaFilterPtr);
					status = ERR_OK;
				}
			}

			return status;
		}

		int MediaGraph::createNewDecoderFilter(const std::string& streamURL)
		{
			int status{ ERR_BAD_ALLOC };
			const std::string streamValue{ URL().getParameter("stream") };

			if (!streamValue.compare("hikvision") || !streamValue.compare("dahua"))
			{
				MediaFilterPtr mediaFilterPtr{ boost::make_shared<AVDecoderFilter>() };
				if (mediaFilterPtr && ERR_OK == mediaFilterPtr->createNewFilter(streamValue))
				{
					mediaFilterGroup.insert(AVMediaSDKDecoderFilterID, mediaFilterPtr);
					status = ERR_OK;
				}
			}
			else
			{
				MediaFilterPtr videoMediaFilterPtr{ boost::make_shared<AVDecoderFilter>() };
				MediaFilterPtr audioMediaFilterPtr{ boost::make_shared<AVDecoderFilter>() };
				if (videoMediaFilterPtr && ERR_OK == videoMediaFilterPtr->createNewFilter("video") &&
					audioMediaFilterPtr && ERR_OK == audioMediaFilterPtr->createNewFilter("audio"))
				{
					mediaFilterGroup.insert(AVMediaVideoDecoderFilterID, videoMediaFilterPtr);
					mediaFilterGroup.insert(AVMediaAudioDecoderFilterID, audioMediaFilterPtr);
					status = ERR_OK;
				}
			}

			return status;
		}

		int MediaGraph::createNewFormatterFilter()
		{
			int status{ ERR_BAD_ALLOC };
			MediaFilterPtr mediaFilterPtr{ boost::make_shared<ImageFormatterFilter>() };
			if (mediaFilterPtr && ERR_OK == mediaFilterPtr->createNewFilter(""))
			{
				mediaFilterGroup.insert(AVMediaImageFormatFilterID, mediaFilterPtr);
				status = ERR_OK;
			}

			return status;
		}

		int MediaGraph::createNewRendererFilter(const std::string& streamURL)
		{
			int status{ ERR_BAD_ALLOC };
			MediaFilterPtr videoMediaFilterPtr{ boost::make_shared<AVRendererFilter>() };
			MediaFilterPtr audioMediaFilterPtr{ boost::make_shared<AVRendererFilter>() };
			if (videoMediaFilterPtr && ERR_OK == videoMediaFilterPtr->createNewFilter("video") &&
				audioMediaFilterPtr && ERR_OK == audioMediaFilterPtr->createNewFilter("audio"))
			{
				mediaFilterGroup.insert(AVMediaVideoRendererFilterID, videoMediaFilterPtr);
				mediaFilterGroup.insert(AVMediaSoundPlayerFilterID, audioMediaFilterPtr);
				status = ERR_OK;
			}

			return status;
		}

		int MediaGraph::createNewCallbackFilter(void)
		{
			int status{ ERR_BAD_ALLOC };
			MediaFilterPtr mediaFilterPtr{ boost::make_shared<AVDataCallbackFilter>() };
			if (mediaFilterPtr && ERR_OK == mediaFilterPtr->createNewFilter(""))
			{
				mediaFilterGroup.insert(AVMediaDataCallbackFilterID, mediaFilterPtr);
				status = ERR_OK;
			}

			return status;
		}

		int MediaGraph::buildMediaGraph()
		{
			MediaPinRef inputMediaPinRef, outputMediaPinRef;
			MediaFilterRef captureFilterRef{ queryMediaFilterByID(AVMediaDataCaptureFilterID) },
				controllerFilterRef{ queryMediaFilterByID(AVMediaPlayControlFilterID) },
				videoDecoderFilterRef{ queryMediaFilterByID(AVMediaVideoDecoderFilterID) },
				audioDecoderFilterRef{ queryMediaFilterByID(AVMediaAudioDecoderFilterID) },
				sdkDecoderFilterRef{ queryMediaFilterByID(AVMediaSDKDecoderFilterID) },
				imageFormatterFilterRef{ queryMediaFilterByID(AVMediaImageFormatFilterID) },
				videoRendererFilterRef{ queryMediaFilterByID(AVMediaVideoRendererFilterID) },
				soundPlayerFilterRef{ queryMediaFilterByID(AVMediaSoundPlayerFilterID) },
				dataCallbackFilterRef{ queryMediaFilterByID(AVMediaDataCallbackFilterID) };

			if (!captureFilterRef.expired() && !controllerFilterRef.expired())
			{
				outputMediaPinRef = captureFilterRef.lock()->queryMediaPinByID(VideoStreamOutputPinID);
				inputMediaPinRef = controllerFilterRef.lock()->queryMediaPinByID(VideoStreamInputPinID);
				if (!outputMediaPinRef.expired() && !inputMediaPinRef.expired())
				{
					outputMediaPinRef.lock()->connectPin(inputMediaPinRef);
				}

				outputMediaPinRef = captureFilterRef.lock()->queryMediaPinByID(AudioStreamOutputPinID);
				inputMediaPinRef = controllerFilterRef.lock()->queryMediaPinByID(AudioStreamInputPinID);
				if (!outputMediaPinRef.expired() && !inputMediaPinRef.expired())
				{
					outputMediaPinRef.lock()->connectPin(inputMediaPinRef);
				}
			}

			if (!controllerFilterRef.expired() && !videoDecoderFilterRef.expired())
			{
				outputMediaPinRef = controllerFilterRef.lock()->queryMediaPinByID(VideoStreamOutputPinID);
				inputMediaPinRef = videoDecoderFilterRef.lock()->queryMediaPinByID(VideoStreamInputPinID);
				if (!outputMediaPinRef.expired() && !inputMediaPinRef.expired())
				{
					outputMediaPinRef.lock()->connectPin(inputMediaPinRef);
				}
			}

			if (!controllerFilterRef.expired() && !audioDecoderFilterRef.expired())
			{
				outputMediaPinRef = controllerFilterRef.lock()->queryMediaPinByID(AudioStreamOutputPinID);
				inputMediaPinRef = audioDecoderFilterRef.lock()->queryMediaPinByID(AudioStreamInputPinID);
				if (!outputMediaPinRef.expired() && !inputMediaPinRef.expired())
				{
					outputMediaPinRef.lock()->connectPin(inputMediaPinRef);
				}
			}

			if (!videoDecoderFilterRef.expired() && !imageFormatterFilterRef.expired())
			{
				outputMediaPinRef = videoDecoderFilterRef.lock()->queryMediaPinByID(VideoStreamOutputPinID);
				inputMediaPinRef = imageFormatterFilterRef.lock()->queryMediaPinByID(VideoStreamInputPinID);
				if (!outputMediaPinRef.expired() && !inputMediaPinRef.expired())
				{
					outputMediaPinRef.lock()->connectPin(inputMediaPinRef);
				}
			}

			if (!captureFilterRef.expired() && !sdkDecoderFilterRef.expired())
			{
				outputMediaPinRef = captureFilterRef.lock()->queryMediaPinByID(VideoStreamOutputPinID);
				inputMediaPinRef = sdkDecoderFilterRef.lock()->queryMediaPinByID(VideoStreamInputPinID);
				if (!outputMediaPinRef.expired() && !inputMediaPinRef.expired())
				{
					outputMediaPinRef.lock()->connectPin(inputMediaPinRef);
				}
			}

			if (!sdkDecoderFilterRef.expired() && !imageFormatterFilterRef.expired() && !soundPlayerFilterRef.expired())
			{
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

			if (!imageFormatterFilterRef.expired() && !videoRendererFilterRef.expired())
			{
				outputMediaPinRef = imageFormatterFilterRef.lock()->queryMediaPinByID(VideoStreamOutputPinID);
				inputMediaPinRef = videoRendererFilterRef.lock()->queryMediaPinByID(VideoStreamInputPinID);
				if (!outputMediaPinRef.expired() && !inputMediaPinRef.expired())
				{
					outputMediaPinRef.lock()->connectPin(inputMediaPinRef);
				}
			}

			if (!audioDecoderFilterRef.expired() && !soundPlayerFilterRef.expired())
			{
				outputMediaPinRef = videoDecoderFilterRef.lock()->queryMediaPinByID(AudioStreamOutputPinID);
				inputMediaPinRef = imageFormatterFilterRef.lock()->queryMediaPinByID(AudioStreamInputPinID);
				if (!outputMediaPinRef.expired() && !inputMediaPinRef.expired())
				{
					outputMediaPinRef.lock()->connectPin(inputMediaPinRef);
				}
			}

			if (!videoRendererFilterRef.expired() && !soundPlayerFilterRef.expired() && !dataCallbackFilterRef.expired())
			{
				outputMediaPinRef = videoRendererFilterRef.lock()->queryMediaPinByID(VideoStreamOutputPinID);
				inputMediaPinRef = dataCallbackFilterRef.lock()->queryMediaPinByID(VideoStreamInputPinID);
				if (!outputMediaPinRef.expired() && !inputMediaPinRef.expired())
				{
					outputMediaPinRef.lock()->connectPin(inputMediaPinRef);
				}

				outputMediaPinRef = soundPlayerFilterRef.lock()->queryMediaPinByID(AudioStreamOutputPinID);
				inputMediaPinRef = dataCallbackFilterRef.lock()->queryMediaPinByID(AudioStreamInputPinID);
				if (!outputMediaPinRef.expired() && !inputMediaPinRef.expired())
				{
					outputMediaPinRef.lock()->connectPin(inputMediaPinRef);
				}
			}

			return ERR_OK;
		}
	}//namespace multimedia
}//namespace framework
