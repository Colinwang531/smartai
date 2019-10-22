#include "error.h"
#include "MediaPin/MediaPin.h"

NS_BEGIN(pin, 1)

MediaPin::MediaPin(const MediaPinMode mode /* = MediaPinMode::MEDIA_PIN_INPUT */) : mediaPinMode{ mode }
{}

MediaPin::~MediaPin()
{}

int MediaPin::connectPin(MediaPinRef inputPinRef)
{
	return ERR_NOT_SUPPORT;
}

int MediaPin::inputData(MediaDataPtr dataPtr)
{
	return dataPtr ? ERR_OK : ERR_INVALID_PARAM;
}

NS_END
