#include "MediaPin/InputMediaPin.h"

NS_BEGIN(pin, 1)

InputMediaPin::InputMediaPin() : MediaPin()
{}

InputMediaPin::~InputMediaPin()
{}

bool InputMediaPin::isInputPin() const
{
	return true;
}

NS_END
