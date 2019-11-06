#include "error.h"
#include "MediaDecoder/FFmpeg/FFmpegVideoDecoder.h"

NS_BEGIN(decoder, 1)

FFmpegVideoDecoder::FFmpegVideoDecoder() : MediaDecoder()
{}

FFmpegVideoDecoder::~FFmpegVideoDecoder(void)
{}

int FFmpegVideoDecoder::decode(const unsigned char* frameData /* = NULL */, const int frameBytes /* = 0 */)
{
	return ERR_NOT_SUPPORT;
}

NS_END
