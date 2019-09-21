#include "Stream/Livestream.h"

NS_BEGIN(stream, 1)

Livestream::Livestream(const unsigned short idx /* = -1 */)
	: AVStream(), livestreamID{ -1 }, cameraIndex{ idx }
{}

Livestream::~Livestream()
{}

NS_END
