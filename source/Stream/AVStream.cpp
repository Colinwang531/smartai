#include "Stream/AVStream.h"

NS_BEGIN(stream, 1)

AVStream::AVStream(const unsigned short idx /* = -1 */) 
	: streamIndex{ idx }, streamStatus{ AVStreamStatus::AVSTREAM_STATUS_STOP }
{}

AVStream::~AVStream()
{}

NS_END
