#include "error.h"
#include "MediaScaler/FrameScaler.h"

NS_BEGIN(scaler, 1)

FrameScaler::FrameScaler()
{}

FrameScaler::~FrameScaler()
{}

const char* FrameScaler::scale(
	const char* data /* = NULL */, const int dataBytes /* = 0 */, const int w /* = 0 */, const int h /* = 0 */)
{
	return data && dataBytes && 0 < w && 0 < h ? data : NULL;
}

NS_END
