#include "error.h"
#include "MediaPin/MediaPin.h"

NS_BEGIN(pin, 1)

MediaPin::MediaPin()
{}

MediaPin::~MediaPin()
{}

int MediaPin::connectInputPin(boost::shared_ptr<MediaPin> inputPinPtr)
{
	return inputPinPtr ? ERR_OK : ERR_INVALID_PARAM;
}

int MediaPin::inputData(MediaDataPtr dataPtr)
{
	return dataPtr ? ERR_OK : ERR_INVALID_PARAM;
}

bool MediaPin::isInputPin() const
{
	return false;
}

bool MediaPin::isOutputPin() const
{
	return false;
}

NS_END
