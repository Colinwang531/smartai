#include "boost/make_shared.hpp"
#include "error.h"
#include "MediaDecoder/FFmpeg/FFmpegVideoDecoder.h"
using FFmpegVideoDecoder = NS(decoder, 1)::FFmpegVideoDecoder;
#include "MediaFilter/Decoder/AVDecoderFilter.h"

NS_BEGIN(filter, 1)

AVDecoderFilter::AVDecoderFilter() : MediaFilter()
{}

AVDecoderFilter::~AVDecoderFilter()
{}

int AVDecoderFilter::createNewMediaDecoder(
	const MediaStreamID streamID /* = MediaStreamID::MEDIA_STREAM_ID_NONE */)
{
	int status{ ERR_OK };

	if (MediaStreamID::MEDIA_STREAM_ID_H264 == streamID || MediaStreamID::MEDIA_STREAM_ID_H265 == streamID)
	{
		MediaDecoderPtr decoderPtr{ boost::make_shared<FFmpegVideoDecoder>() };
		if (decoderPtr)
		{
			mediaDecoderPtr.swap(decoderPtr);
		}
	}
	else if (MediaStreamID::MEDIA_STREAM_ID_AAC == streamID)
	{
	}
	else if (MediaStreamID::MEDIA_STREAM_ID_G722 == streamID)
	{
	}
	else
	{
		status = ERR_NOT_SUPPORT;
	}

	return ERR_OK == status ? createNewInputAndOutputPin(streamID) : status;
}

int AVDecoderFilter::inputMediaData(MediaDataPtr mediaData)
{
	int status{ mediaDecoderPtr ? ERR_OK : ERR_BAD_OPERATE };

// 	if (ERR_OK == status)
// 	{
// 		// if current media data is the first one, create demuxer and output pins instances once.
// 		status = createNewMediaDemuxer(mediaData);
// 
// 		if (ERR_OK == status)
// 		{
// 			status = createNewOutputPin();
// 		}
// 	}

	return status;
}

int AVDecoderFilter::createNewInputAndOutputPin(
	const MediaStreamID streamID /* = MediaStreamID::MEDIA_STREAM_ID_NONE */)
{
	int status{ ERR_OK };

	if (MediaStreamID::MEDIA_STREAM_ID_H264 == streamID || MediaStreamID::MEDIA_STREAM_ID_H265 == streamID)
	{
		createNewInputPin(NS(pin, 1)::VideoStreamInputPinID);
		createNewOutputPin(NS(pin, 1)::VideoStreamOutputPinID);
	} 
	else if (MediaStreamID::MEDIA_STREAM_ID_AAC == streamID || MediaStreamID::MEDIA_STREAM_ID_G722 == streamID)
	{
		createNewInputPin(NS(pin, 1)::AudioStreamInputPinID);
		createNewOutputPin(NS(pin, 1)::AudioStreamOutputPinID);
	}
	else
	{
		status = ERR_NOT_SUPPORT;
	}

	return status;
}

NS_END
