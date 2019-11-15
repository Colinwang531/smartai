#include "error.h"
#include "MediaModel/Decoder/FFmpeg/FFmpegAudioDecoder.h"

NS_BEGIN(model, 1)

FFmpegAudioDecoder::FFmpegAudioDecoder() : MediaDecoder()
{}

FFmpegAudioDecoder::~FFmpegAudioDecoder(void)
{}

int FFmpegAudioDecoder::inputMediaData(MediaDataPtr mediaData)
{
	return ERR_NOT_SUPPORT;
}

NS_END
