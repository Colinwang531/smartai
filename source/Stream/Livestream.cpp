#include "Stream/Livestream.h"

NS_BEGIN(stream, 1)

Livestream::Livestream(const unsigned short idx /* = -1 */) : AVStream(idx), streamID{ -1 }
{}

Livestream::~Livestream()
{}

void Livestream::captureVideoStreamProcess(
	const unsigned char* data /*= NULL*/, const long long dataBytes /*= 0*/)
{}

void Livestream::captureAudioStreamProcess(
	const unsigned char* data /*= NULL*/, const long long dataBytes /*= 0*/)
{}

NS_END
