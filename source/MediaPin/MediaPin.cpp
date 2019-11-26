#include "error.h"
#include "MediaPin/MediaPin.h"

NS_BEGIN(pin, 1)

MediaPin::MediaPin()
{}

MediaPin::~MediaPin()
{}

int MediaPin::inputData(MediaDataPtr dataPtr)
{
	return dataPtr ? ERR_OK : ERR_INVALID_PARAM;
}

NS_END
