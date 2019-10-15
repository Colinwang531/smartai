#include "MediaPin/OutputMediaPin.h"

NS_BEGIN(pin, 1)

OutputMediaPin::OutputMediaPin() : MediaPin()
{}

OutputMediaPin::~OutputMediaPin()
{}

bool OutputMediaPin::isOutputPin() const
{
	return true;
}

NS_END
