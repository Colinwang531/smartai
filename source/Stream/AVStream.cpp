#include "Stream/AVStream.h"

NS_BEGIN(stream, 1)

AVStream::AVStream(const unsigned short idx /* = -1 */) : streamIndex{ idx }
{}

AVStream::~AVStream()
{}

NS_END
