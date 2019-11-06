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
	const MediaDecodeType decodeType /* = MediaDecodeType::MEDIA_DECODE_TYPE_H2645 */)
{
	int status{ ERR_OK };

	if (MediaDecodeType::MEDIA_DECODE_TYPE_H2645 == decodeType)
	{
		MediaDecoderPtr decoderPtr{ boost::make_shared<FFmpegVideoDecoder>() };
		if (decoderPtr)
		{
			mediaDecoderPtr.swap(decoderPtr);
		}
	}
	else if (MediaDecodeType::MEDIA_DECODE_TYPE_AAC == decodeType)
	{
	}
	else if (MediaDecodeType::MEDIA_DECODE_TYPE_G722 == decodeType)
	{
	}
	else
	{
		status = ERR_NOT_SUPPORT;
	}

	return ERR_OK == status ? createNewInputAndOutputPin() : status;
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
	const MediaDecodeType decodeType /* = MediaDecodeType::MEDIA_DECODE_TYPE_H2645 */)
{
	int status{ ERR_OK };

	if (MediaDecodeType::MEDIA_DECODE_TYPE_H2645 == decodeType)
	{
		createNewInputPin(NS(pin, 1)::VideoStreamInputPinID);
		createNewOutputPin(NS(pin, 1)::VideoStreamOutputPinID);
	} 
	else if (MediaDecodeType::MEDIA_DECODE_TYPE_AAC == decodeType || MediaDecodeType::MEDIA_DECODE_TYPE_G722 == decodeType)
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
