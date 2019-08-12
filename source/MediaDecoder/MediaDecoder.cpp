#include "error.h"
#include "MediaDecoder/MediaDecoder.h"

NS_BEGIN(decoder, 1)

MediaDecoder::MediaDecoder()
{}

MediaDecoder::~MediaDecoder()
{}

int MediaDecoder::decode(const char* data /* = NULL */, const int dataBytes /* = 0 */)
{
	return data && 0 < dataBytes ? ERR_OK : ERR_INVALID_PARAM;
}

NS_END
