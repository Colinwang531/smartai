#include "boost/make_shared.hpp"
#include "error.h"
#include "MediaPin/InputMediaPin.h"
#include "MediaPin/OutputMediaPin.h"
#include "MediaFilter/Controller/AVPlayControllerFilter.h"

namespace framework
{
	namespace multimedia
	{
		AVPlayControllerFilter::AVPlayControllerFilter() : MediaFilter()
		{}

		AVPlayControllerFilter::~AVPlayControllerFilter()
		{}

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

		int AVPlayControllerFilter::createNewFilter()
		{
			return ERR_OK == MediaFilter::createNewInputPin(VideoStreamInputPinID) &&
				ERR_OK == MediaFilter::createNewInputPin(AudioStreamInputPinID) &&
				ERR_OK == MediaFilter::createNewOutputPin(VideoStreamOutputPinID) &&
				ERR_OK == MediaFilter::createNewOutputPin(AudioStreamOutputPinID) ? ERR_OK : ERR_BAD_ALLOC;
		}

		int AVPlayControllerFilter::createNewModule(MediaDataPtr mediaData)
		{
			return ERR_NOT_IMPLEMENT;
		}

		int AVPlayControllerFilter::checkSpeedValue(const short speed /* = 1 */)
		{
			return 1 == speed || 2 == speed || 4 == speed || 8 == speed || 16 == speed ||
				-1 == speed || -2 == speed || -4 == speed || -8 == speed || -16 == speed ?
				ERR_OK : ERR_INVALID_PARAM;
		}
	}//namespace multimedia
}//namespace framework
