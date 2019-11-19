#include "boost/make_shared.hpp"
#include "error.h"
#include "MediaModel/Decoder/FFmpeg/FFmpegVideoDecoder.h"
using FFmpegVideoDecoder = NS(model, 1)::FFmpegVideoDecoder;
#include "MediaModel/Decoder/FFmpeg/FFmpegAudioDecoder.h"
using FFmpegAudioDecoder = NS(model, 1)::FFmpegAudioDecoder;
#include "MediaFilter/Decoder/AVDecoderFilter.h"

NS_BEGIN(filter, 1)

AVDecoderFilter::AVDecoderFilter(const AVDecoderType type /* = AVDecoderType::AV_DECODER_TYPE_NONE */)
	: MediaFilter(), decoderType{ type }
{}

AVDecoderFilter::~AVDecoderFilter()
{}

int AVDecoderFilter::createNewFilter()
{
	int status{ ERR_BAD_ALLOC };

	if (AVDecoderType::AV_DECODER_TYPE_VIDEO == decoderType)
	{
		if (ERR_OK == createNewInputPin(NS(pin, 1)::VideoStreamInputPinID) &&
			ERR_OK == createNewOutputPin(NS(pin, 1)::VideoStreamOutputPinID))
		{
			MediaModelPtr videoDecoderPtr{ boost::make_shared<FFmpegVideoDecoder>() };
			if (videoDecoderPtr)
			{
				mediaModelPtr.swap(videoDecoderPtr);
				status = ERR_OK;
			}
		}
	} 
	else if (AVDecoderType::AV_DECODER_TYPE_AUDIO == decoderType)
	{
		if (ERR_OK == createNewInputPin(NS(pin, 1)::AudioStreamInputPinID) &&
			ERR_OK == createNewOutputPin(NS(pin, 1)::AudioStreamOutputPinID))
		{
			MediaModelPtr audioDecoderPtr{ boost::make_shared<FFmpegAudioDecoder>() };
			if (audioDecoderPtr)
			{
				mediaModelPtr.swap(audioDecoderPtr);
				status = ERR_OK;
			}
		}
	}

	return ERR_OK == status ? MediaFilter::createNewFilter() : status;
}

int AVDecoderFilter::destroyFilter()
{
	return ERR_OK;
}

int AVDecoderFilter::inputMediaData(MediaDataPtr mediaData)
{
	return mediaData && mediaModelPtr ? mediaModelPtr->inputMediaData(mediaData) : ERR_INVALID_PARAM;
}

NS_END
