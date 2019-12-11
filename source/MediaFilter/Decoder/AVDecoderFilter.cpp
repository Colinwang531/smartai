#include "boost/make_shared.hpp"
#include "error.h"
#include "MediaData/MediaData.h"
#include "MediaModule/Decoder/FFmpeg/FFmpegVideoDecoder.h"
#include "MediaModule/Decoder/FFmpeg/FFmpegAudioDecoder.h"
#include "MediaModule/Decoder/SDK/HikvisionSDKDecoder.h"
#include "MediaPin/MediaPin.h"
#include "MediaFilter/Decoder/AVDecoderFilter.h"

namespace framework
{
	namespace multimedia
	{
		AVDecoderFilter::AVDecoderFilter(
			const DecoderType type /* = DecoderType::DECODER_TYPE_NONE */)
			: MediaFilter(), decoderType{ type }
		{}

		AVDecoderFilter::~AVDecoderFilter()
		{}

		int AVDecoderFilter::createNewFilter()
		{
			int status{ ERR_INVALID_PARAM };

			if (DecoderType::DECODER_TYPE_DAHUA == decoderType || DecoderType::DECODER_TYPE_HIKVISION == decoderType)
			{
				status = ERR_OK == MediaFilter::createNewInputPin(AudioStreamInputPinID) &&
					ERR_OK == MediaFilter::createNewOutputPin(AudioStreamOutputPinID) &&
					ERR_OK == MediaFilter::createNewInputPin(VideoStreamInputPinID) &&
					ERR_OK == MediaFilter::createNewOutputPin(VideoStreamOutputPinID) ? ERR_OK : ERR_BAD_ALLOC;
			}
			else if (DecoderType::DECODER_TYPE_H264 == decoderType || DecoderType::DECODER_TYPE_H265 == decoderType || DecoderType::DECODER_TYPE_RTSP_RTP == decoderType)
			{
				status = ERR_OK == MediaFilter::createNewInputPin(VideoStreamInputPinID) &&
					ERR_OK == MediaFilter::createNewOutputPin(VideoStreamOutputPinID) ? ERR_OK : ERR_BAD_ALLOC;
			}
			else if (DecoderType::DECODER_TYPE_AAC == decoderType)
			{
				status = ERR_OK == MediaFilter::createNewInputPin(AudioStreamInputPinID) &&
					ERR_OK == MediaFilter::createNewOutputPin(AudioStreamOutputPinID) ? ERR_OK : ERR_BAD_ALLOC;
			}

			return status;
		}

		int AVDecoderFilter::createNewModule(MediaDataPtr mediaData)
		{
			int status{ mediaData ? ERR_OK : ERR_INVALID_PARAM };

			if (ERR_OK == status)
			{
				MediaModulePtr decoderModulePtr;
				const MediaDataMainID mediaDataMainID{ mediaData->getMainID() };
				const MediaDataSubID mediaDataSubID{ mediaData->getSubID() };

				if (MediaDataMainID::MEDIA_DATA_MAIN_ID_VIDEO == mediaDataMainID)
				{
					if (MediaDataSubID::MEDIA_DATA_SUB_ID_H264 == mediaDataSubID || MediaDataSubID::MEDIA_DATA_SUB_ID_H265 == mediaDataSubID)
					{
						decoderModulePtr = boost::make_shared<FFmpegVideoDecoder>();
					}
					else if (MediaDataSubID::MEDIA_DATA_SUB_ID_HIKVISION == mediaDataSubID)
					{
						decoderModulePtr = boost::make_shared<HikvisionSDKDecoder>();
					}
				}
				else if (MediaDataMainID::MEDIA_DATA_MAIN_ID_AUDIO == mediaDataMainID)
				{
					decoderModulePtr = boost::make_shared<FFmpegAudioDecoder>();
				}
				else
				{
					status = ERR_NOT_SUPPORT;
				}

				if (decoderModulePtr)
				{
					mediaModulePtr.swap(decoderModulePtr);
					MediaFilter::setPostInputMediaDataCallback();
				}
			}

			return status;
		}
	}//namespace multimedia
}//namespace framework
