#include "boost/make_shared.hpp"
#include "error.h"
#include "Url/Url.h"
using URL = framework::wrapper::URL;
#include "MediaPin/MediaPin.h"
#include "MediaFilter/Capture/AVCaptureFilter.h"
#include "MediaFilter/Controller/AVPlayControllerFilter.h"
#include "MediaFilter/Decoder/AVDecoderFilter.h"
#include "MediaFilter/Converter/AVConverterFilter.h"
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
				ERR_OK == createNewCaptureFilter() &&
				ERR_OK == createNewControllerFilter(streamURL) &&
				ERR_OK == createNewDecoderFilter(streamURL) &&
				ERR_OK == createNewConverterFilter() &&
				ERR_OK == createNewRendererFilter(hwnd) &&
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

		int MediaGraph::createNewCaptureFilter()
		{
			int status{ ERR_BAD_ALLOC };
			MediaFilterPtr mediaFilterPtr{ boost::make_shared<AVCaptureFilter>() };
			if (mediaFilterPtr && ERR_OK == mediaFilterPtr->createNewFilter())
			{
				mediaFilterGroup.insert(AVMediaDataCaptureFilterID, mediaFilterPtr);
				status = ERR_OK;
			}

			return status;
		}

		int MediaGraph::createNewControllerFilter(const std::string& streamURL)
		{
			const framework::wrapper::URLProtocol protocol{ URL(streamURL).getProtocol() };
			if (framework::wrapper::URLProtocol::URL_PROTOCOL_PLAYBACK == protocol)
			{
				MediaFilterPtr mediaFilterPtr{ boost::make_shared<AVPlayControllerFilter>() };
				if (mediaFilterPtr && ERR_OK == mediaFilterPtr->createNewFilter())
				{
					mediaFilterGroup.insert(AVMediaPlayControlFilterID, mediaFilterPtr);
				}
			}

			return ERR_OK;
		}

		int MediaGraph::createNewDecoderFilter(const std::string& streamURL)
		{
			int status{ ERR_BAD_ALLOC };
			const std::string streamValue{ URL(streamURL).getParameter("stream") };
			const DecoderType decoderType{ (const DecoderType)atoi(streamValue.c_str()) };

			if (DecoderType::DECODER_TYPE_HIKVISION == decoderType || DecoderType::DECODER_TYPE_DAHUA == decoderType)
			{
				MediaFilterPtr mediaFilterPtr{ boost::make_shared<AVDecoderFilter>(decoderType) };
				if (mediaFilterPtr && ERR_OK == mediaFilterPtr->createNewFilter())
				{
					mediaFilterGroup.insert(AVMediaSDKDecoderFilterID, mediaFilterPtr);
					status = ERR_OK;
				}
			}
			else
			{
				MediaFilterPtr videoMediaFilterPtr{ boost::make_shared<AVDecoderFilter>(decoderType) };
				MediaFilterPtr audioMediaFilterPtr{ boost::make_shared<AVDecoderFilter>(DecoderType::DECODER_TYPE_AAC) };
				if (videoMediaFilterPtr && ERR_OK == videoMediaFilterPtr->createNewFilter() &&
					audioMediaFilterPtr && ERR_OK == audioMediaFilterPtr->createNewFilter())
				{
					mediaFilterGroup.insert(AVMediaVideoDecoderFilterID, videoMediaFilterPtr);
					mediaFilterGroup.insert(AVMediaAudioDecoderFilterID, audioMediaFilterPtr);
					status = ERR_OK;
				}
			}

			return status;
		}

		int MediaGraph::createNewConverterFilter()
		{
			int status{ ERR_BAD_ALLOC };
			MediaFilterPtr mediaFilterPtr{ boost::make_shared<AVConverterFilter>() };
			if (mediaFilterPtr && ERR_OK == mediaFilterPtr->createNewFilter())
			{
				mediaFilterGroup.insert(AVMediaImageFormatFilterID, mediaFilterPtr);
				status = ERR_OK;
			}

			return status;
		}

		int MediaGraph::createNewRendererFilter(void* hwnd /* = NULL */)
		{
			int status{ ERR_BAD_ALLOC };
			MediaFilterPtr videoMediaFilterPtr{ boost::make_shared<AVRendererFilter>(RendererType::RENDERER_TYPE_VIDEO) };
			MediaFilterPtr audioMediaFilterPtr{ boost::make_shared<AVRendererFilter>(RendererType::RENDERER_TYPE_AUDIO) };
			if (videoMediaFilterPtr && ERR_OK == videoMediaFilterPtr->createNewFilter() &&
				audioMediaFilterPtr && ERR_OK == audioMediaFilterPtr->createNewFilter())
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
			if (mediaFilterPtr && ERR_OK == mediaFilterPtr->createNewFilter())
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
				imageFormatterFilterRef{ queryMediaFilterByID(AVMediaImageFormatFilterID) },
				videoRendererFilterRef{ queryMediaFilterByID(AVMediaVideoRendererFilterID) },
				soundPlayerFilterRef{ queryMediaFilterByID(AVMediaSoundPlayerFilterID) },
				dataCallbackFilterRef{ queryMediaFilterByID(AVMediaDataCallbackFilterID) };

			if (captureFilterRef.expired() || imageFormatterFilterRef.expired())
			{
				return ERR_BAD_ALLOC;
			}

			if (controllerFilterRef.expired())
			{
				MediaFilterRef sdkDecoderFilterRef{ queryMediaFilterByID(AVMediaSDKDecoderFilterID) };
				outputMediaPinRef = captureFilterRef.lock()->queryMediaPinByID(VideoStreamOutputPinID);
				inputMediaPinRef = sdkDecoderFilterRef.lock()->queryMediaPinByID(VideoStreamInputPinID);
				if (!outputMediaPinRef.expired() && !inputMediaPinRef.expired())
				{
					outputMediaPinRef.lock()->connectPin(inputMediaPinRef);
				}

				outputMediaPinRef = captureFilterRef.lock()->queryMediaPinByID(AudioStreamOutputPinID);
				inputMediaPinRef = sdkDecoderFilterRef.lock()->queryMediaPinByID(AudioStreamInputPinID);
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
			else
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

			outputMediaPinRef = imageFormatterFilterRef.lock()->queryMediaPinByID(VideoStreamOutputPinID);
			inputMediaPinRef = videoRendererFilterRef.lock()->queryMediaPinByID(VideoStreamInputPinID);
			if (!outputMediaPinRef.expired() && !inputMediaPinRef.expired())
			{
				outputMediaPinRef.lock()->connectPin(inputMediaPinRef);
			}

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

			return ERR_OK;
		}
	}//namespace multimedia
}//namespace framework
