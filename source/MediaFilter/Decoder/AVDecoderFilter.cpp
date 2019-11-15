#include "boost/make_shared.hpp"
#include "error.h"
#include "MediaModel/Decoder/FFmpeg/FFmpegVideoDecoder.h"
using FFmpegVideoDecoder = NS(model, 1)::FFmpegVideoDecoder;
#include "MediaModel/Decoder/FFmpeg/FFmpegAudioDecoder.h"
using FFmpegAudioDecoder = NS(model, 1)::FFmpegAudioDecoder;
#include "MediaFilter/Decoder/AVDecoderFilter.h"

NS_BEGIN(filter, 1)

AVDecoderFilter::AVDecoderFilter(const MediaDataSubID subID /* = MediaDataSubID::MEDIA_DATA_SUB_ID_NONE */)
	: MediaFilter(), mediaDataSubID{ subID }
{}

AVDecoderFilter::~AVDecoderFilter()
{}

int AVDecoderFilter::createNewFilter()
{
	if (MediaDataSubID::MEDIA_DATA_SUB_ID_H264 == mediaDataSubID || MediaDataSubID::MEDIA_DATA_SUB_ID_H265 == mediaDataSubID)
	{
		MediaModelPtr ffmpegVideoDecoderPtr{ boost::make_shared<FFmpegVideoDecoder>() };
		if (ffmpegVideoDecoderPtr)
		{
			createNewInputPin(NS(pin, 1)::VideoStreamInputPinID);
			createNewOutputPin(NS(pin, 1)::VideoStreamOutputPinID);
			mediaModelPtr.swap(ffmpegVideoDecoderPtr);
		}
	}
	else if (MediaDataSubID::MEDIA_DATA_SUB_ID_AAC == mediaDataSubID)
	{
		MediaModelPtr ffmpegAudioDecoderPtr{ boost::make_shared<FFmpegAudioDecoder>() };
		if (ffmpegAudioDecoderPtr)
		{
			createNewInputPin(NS(pin, 1)::AudioStreamInputPinID);
			createNewOutputPin(NS(pin, 1)::AudioStreamOutputPinID);
			mediaModelPtr.swap(ffmpegAudioDecoderPtr);
		}
	}
	else if (MediaDataSubID::MEDIA_DATA_SUB_ID_G722 == mediaDataSubID)
	{
	}
	else
	{
	}

	return MediaFilter::createNewFilter();
}

int AVDecoderFilter::destroyFilter()
{
	return ERR_OK;
}

int AVDecoderFilter::inputMediaData(MediaDataPtr mediaData)
{
	return mediaData ? mediaModelPtr->inputMediaData(mediaData) : ERR_INVALID_PARAM;
}

NS_END
