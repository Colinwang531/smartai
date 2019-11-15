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
	createNewInputPin(NS(pin, 1)::VideoStreamInputPinID);
	createNewInputPin(NS(pin, 1)::AudioStreamInputPinID);
	createNewOutputPin(NS(pin, 1)::VideoStreamOutputPinID);
	createNewOutputPin(NS(pin, 1)::AudioStreamOutputPinID);
	return MediaFilter::createNewFilter();
}

int AVPlayControllerFilter::destroyFilter()
{
	return ERR_OK;
}

int AVPlayControllerFilter::inputMediaData(MediaDataPtr mediaData)
{
	return postMediaDataCallback(mediaData);
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
