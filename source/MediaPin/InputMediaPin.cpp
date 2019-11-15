#include "error.h"
#include "MediaPin/InputMediaPin.h"

NS_BEGIN(pin, 1)

InputMediaPin::InputMediaPin(MediaFilterRef filterRef)
	: MediaPin(), parentMediaFilterRef{ filterRef }
{}

InputMediaPin::~InputMediaPin()
{}

int InputMediaPin::inputData(MediaDataPtr dataPtr)
{
	int status{ MediaPin::inputData(dataPtr) };

	if (ERR_OK == status)
	{
		status = parentMediaFilterRef.expired() ? 
			ERR_BAD_OPERATE : parentMediaFilterRef.lock()->inputMediaData(dataPtr);
	}

	return status;
}

NS_END
