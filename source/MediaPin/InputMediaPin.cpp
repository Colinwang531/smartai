#include "error.h"
#include "MediaPin/InputMediaPin.h"

NS_BEGIN(pin, 1)

InputMediaPin::InputMediaPin(MediaFilterPtr filterPtr) 
	: MediaPin(), mediaFilterPtr{ filterPtr }
{}

InputMediaPin::~InputMediaPin()
{}

int InputMediaPin::connectInputPin(boost::shared_ptr<MediaPin> inputPinPtr)
{
	return ERR_NOT_SUPPORT;
}

int InputMediaPin::inputData(MediaDataPtr dataPtr)
{
	int status{ MediaPin::inputData(dataPtr) };

	if (ERR_OK == status)
	{
		status = mediaFilterPtr ? mediaFilterPtr->inputData(dataPtr) : ERR_BAD_OPERATE;
	}

	return status;
}

bool InputMediaPin::isInputPin() const
{
	return true;
}

NS_END
