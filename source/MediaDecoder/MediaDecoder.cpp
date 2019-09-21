#include "error.h"
#include "MediaDecoder/MediaDecoder.h"

NS_BEGIN(decoder, 1)

MediaDecoder::MediaDecoder()
{}

MediaDecoder::~MediaDecoder()
{}

int MediaDecoder::decode(
	const unsigned char* frameData /* = NULL */, const int frameBytes /* = 0 */, const unsigned long long frameNumber /* = 0 */)
{
	int status{ ERR_INVALID_PARAM };

	if (frameData && 0 < frameBytes/* && 0 < frameNumber*/)
	{
		status = ERR_OK;
	}

	return status;
}

NS_END
