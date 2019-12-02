#include "boost/make_shared.hpp"
#include "error.h"
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

		int AVPlayControllerFilter::checkSpeedValue(const short speed /* = 1 */)
		{
			return 1 == speed || 2 == speed || 4 == speed || 8 == speed || 16 == speed ||
				-1 == speed || -2 == speed || -4 == speed || -8 == speed || -16 == speed ?
				ERR_OK : ERR_INVALID_PARAM;
		}
	}//namespace multimedia
}//namespace framework
