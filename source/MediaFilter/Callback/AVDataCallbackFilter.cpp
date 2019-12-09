#include "boost/make_shared.hpp"
#include "error.h"
#include "MediaPin/MediaPin.h"
#include "MediaFilter/Callback/AVDataCallbackFilter.h"

namespace framework
{
	namespace multimedia
	{
		AVDataCallbackFilter::AVDataCallbackFilter() : TargetMediaFilter()
		{}

		AVDataCallbackFilter::~AVDataCallbackFilter()
		{}

		int AVDataCallbackFilter::createNewFilter(const std::string & streamURL)
		{
			int status{ ERR_BAD_ALLOC };

			if (ERR_OK == MediaFilter::createNewInputPin(VideoStreamInputPinID) &&
				ERR_OK == MediaFilter::createNewInputPin(AudioStreamInputPinID))
			{
				status = ERR_OK;
			}

			return status;
		}
	}//namespace multimedia
}//namespace framework
