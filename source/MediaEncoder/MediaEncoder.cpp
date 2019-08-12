#include "error.h"
#include "MediaEncoder/MediaEncoder.h"

NS_BEGIN(encoder, 1)

MediaEncoder::MediaEncoder()
{}

MediaEncoder::~MediaEncoder()
{}

int MediaEncoder::encode(char*& jpegData, int& jpegBytes,
	const char* data /* = NULL */, const int dataBytes /* = 0 */, const int w /* = 1920 */, const int h /* = 1080 */)
{
	return data && 0 < dataBytes && 0 < w && 0 < h ? ERR_OK : ERR_INVALID_PARAM;
}

NS_END
