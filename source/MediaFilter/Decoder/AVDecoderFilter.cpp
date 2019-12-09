#include "boost/make_shared.hpp"
#include "error.h"
#include "MediaData/MediaData.h"
#include "MediaModule/Decoder/FFmpeg/FFmpegVideoDecoder.h"
#include "MediaModule/Decoder/FFmpeg/FFmpegAudioDecoder.h"
#include "MediaModule/Decoder/SDK/HikvisionSDKDecoder.h"
#include "MediaPin/OutputMediaPin.h"
#include "MediaFilter/Decoder/AVDecoderFilter.h"

namespace framework
{
	namespace multimedia
	{
		AVDecoderFilter::AVDecoderFilter() : MediaFilter()
		{}

		AVDecoderFilter::~AVDecoderFilter()
		{}

		int AVDecoderFilter::createNewFilter(const std::string& streamURL)
		{
			int status{ ERR_INVALID_PARAM };

			if (!streamURL.compare("hikvision") || !streamURL.compare("dahua"))
			{
				if (ERR_OK == MediaFilter::createNewOutputPin(AudioStreamOutputPinID) &&
					ERR_OK == MediaFilter::createNewInputPin(VideoStreamInputPinID) &&
					ERR_OK == MediaFilter::createNewOutputPin(VideoStreamOutputPinID))
				{
					status = ERR_OK;
				}
			}
			else
			{
				std::string inputPinID, outputPinID;
				if (!streamURL.compare("video"))
				{
					inputPinID = VideoStreamInputPinID;
					outputPinID = VideoStreamOutputPinID;
				}
				else if (!streamURL.compare("audio"))
				{
					inputPinID = AudioStreamInputPinID;
					outputPinID = AudioStreamOutputPinID;
				}

				if (!inputPinID.empty() && !outputPinID.empty() &&
					ERR_OK == MediaFilter::createNewInputPin(inputPinID) &&
					ERR_OK == MediaFilter::createNewOutputPin(outputPinID))
				{
					status = ERR_OK;
				}
			}

			return status;
		}

		int AVDecoderFilter::createNewModel(MediaDataPtr mediaData)
		{
			int status{ mediaData ? ERR_OK : ERR_INVALID_PARAM };

			if (ERR_OK == status)
			{
				const MediaDataMainID mediaDataMainID{ mediaData->getMainID() };
				const MediaDataSubID mediaDataSubID{ mediaData->getSubID() };

				if (MediaDataMainID::MEDIA_DATA_MAIN_ID_VIDEO == mediaDataMainID)
				{
					MediaModelPtr videoDecoderPtr;
					if (MediaDataSubID::MEDIA_DATA_SUB_ID_H264 == mediaDataSubID || MediaDataSubID::MEDIA_DATA_SUB_ID_H265 == mediaDataSubID)
					{
						videoDecoderPtr = boost::make_shared<FFmpegVideoDecoder>();
					}
					else if (MediaDataSubID::MEDIA_DATA_SUB_ID_HIKVISION == mediaDataSubID)
					{
						videoDecoderPtr = boost::make_shared<HikvisionSDKDecoder>();
					}

					if (videoDecoderPtr)
					{
						mediaModelPtr.swap(videoDecoderPtr);
					}
				}
				if (MediaDataMainID::MEDIA_DATA_MAIN_ID_AUDIO == mediaDataMainID)
				{
					MediaModelPtr audioDecoderPtr{ boost::make_shared<FFmpegAudioDecoder>() };
					if (audioDecoderPtr)
					{
						mediaModelPtr.swap(audioDecoderPtr);
					}
				}
			}

			return ERR_OK == status ? MediaFilter::createNewModel(mediaData) : status;
		}
	}//namespace multimedia
}//namespace framework
