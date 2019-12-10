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

		int AVDataCallbackFilter::createNewFilter()
		{
			return ERR_OK == MediaFilter::createNewInputPin(VideoStreamInputPinID) &&
				ERR_OK == MediaFilter::createNewInputPin(AudioStreamInputPinID) ? ERR_OK : ERR_BAD_ALLOC;
		}
	}//namespace multimedia
}//namespace framework
