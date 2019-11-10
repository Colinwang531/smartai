#include "boost/make_shared.hpp"
#include "error.h"
#include "MediaPin/OutputMediaPin.h"
using OutputMediaPin = NS(pin, 1)::OutputMediaPin;
#include "MediaFilter/Controller/AVPlayControllerFilter.h"

NS_BEGIN(filter, 1)

AVPlayControllerFilter::AVPlayControllerFilter() : MediaFilter()
{}

AVPlayControllerFilter::~AVPlayControllerFilter()
{}

int AVPlayControllerFilter::createNewMediaController(void)
{
	createNewInputPin(NS(pin, 1)::VideoStreamInputPinID);
	createNewInputPin(NS(pin, 1)::AudioStreamInputPinID);
	createNewInputPin(NS(pin, 1)::VideoStreamOutputPinID);
	createNewInputPin(NS(pin, 1)::AudioStreamOutputPinID);
	return ERR_OK;
}

int AVPlayControllerFilter::inputMediaData(MediaDataPtr mediaData)
{
	return 0;
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
