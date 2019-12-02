#include "boost/make_shared.hpp"
#include "error.h"
#include "MediaData/MediaData.h"
#include "MediaModel/Decoder/FFmpeg/FFmpegVideoDecoder.h"
#include "MediaModel/Decoder/FFmpeg/FFmpegAudioDecoder.h"
#include "MediaFilter/Decoder/AVDecoderFilter.h"

namespace framework
{
	namespace multimedia
	{
		AVDecoderFilter::AVDecoderFilter() : MediaFilter()
		{}

		AVDecoderFilter::~AVDecoderFilter()
		{}

		int AVDecoderFilter::createNewModel(MediaDataPtr mediaData)
		{
			int status{ mediaData ? ERR_OK : ERR_INVALID_PARAM };

			if (ERR_OK == status)
			{
				const MediaDataMainID mediaDataMainID{ mediaData->getMainID() };
				const MediaDataSubID mediaDataSubID{ mediaData->getSubID() };

				if (MediaDataMainID::MEDIA_DATA_MAIN_ID_VIDEO == mediaDataMainID)
				{
					MediaModelPtr videoDecoderPtr{ boost::make_shared<FFmpegVideoDecoder>() };
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
