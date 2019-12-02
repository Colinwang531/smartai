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
	}//namespace multimedia
}//namespace framework
