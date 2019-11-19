#include "boost/make_shared.hpp"
#include "error.h"
#include "MediaPin/MediaPin.h"
#include "MediaFilter/Controller/AVPlayControllerFilter.h"

NS_BEGIN(filter, 1)

AVPlayControllerFilter::AVPlayControllerFilter() : MediaFilter()
{}

AVPlayControllerFilter::~AVPlayControllerFilter()
{}

int AVPlayControllerFilter::createNewFilter(void)
{
	int status{ ERR_BAD_ALLOC };

	if (ERR_OK == createNewInputPin(NS(pin, 1)::VideoStreamInputPinID) && 
		ERR_OK == createNewInputPin(NS(pin, 1)::AudioStreamInputPinID) &&
		ERR_OK == createNewOutputPin(NS(pin, 1)::VideoStreamOutputPinID) &&
		ERR_OK == createNewOutputPin(NS(pin, 1)::AudioStreamOutputPinID))
	{
		status = MediaFilter::createNewFilter();
	}

	return status;
}

int AVPlayControllerFilter::destroyFilter()
{
	return ERR_OK;
}

int AVPlayControllerFilter::inputMediaData(MediaDataPtr mediaData)
{
	return MediaFilter::postInputMediaData(mediaData);
}

int AVPlayControllerFilter::startPlay()
{
	return ERR_OK;
}

int AVPlayControllerFilter::stopPlay()
{
	return ERR_OK;
}

int AVPlayControllerFilter::pausePlay()
{
	return ERR_OK;
}

int AVPlayControllerFilter::fastPlay(const short speed /* = 1 */)
{
	return checkSpeedValue(speed);
}

int AVPlayControllerFilter::slowPlay(const short speed /* = -1 */)
{
	return checkSpeedValue(speed);
}

int AVPlayControllerFilter::checkSpeedValue(const short speed /* = 1 */)
{
	return 1 == speed || 2 == speed || 4 == speed || 8 == speed || 16 == speed ||
		-1 == speed || -2 == speed || -4 == speed || -8 == speed || -16 == speed ?
		ERR_OK : ERR_INVALID_PARAM;
}

NS_END
