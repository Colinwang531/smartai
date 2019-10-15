#include "MediaPin/MediaPin.h"

NS_BEGIN(pin, 1)

MediaPin::MediaPin()
{}

MediaPin::~MediaPin()
{}

bool MediaPin::isInputPin() const
{
	return false;
}

bool MediaPin::isOutputPin() const
{
	return false;
}

NS_END
