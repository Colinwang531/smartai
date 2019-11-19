#include "boost/make_shared.hpp"
#include "error.h"
#include "MediaModel/Accessor/SDK/EatonIPCMediaAccessor.h"
using EatonIPCMediaAccessor = NS(model, 1)::EatonIPCMediaAccessor;
#include "MediaFilter/Capture/LivestreamCaptureFilter.h"

NS_BEGIN(filter, 1)

LivestreamCaptureFilter::LivestreamCaptureFilter() : SourceMediaFilter()
{}

LivestreamCaptureFilter::~LivestreamCaptureFilter()
{}

int LivestreamCaptureFilter::createNewFilter()
{
	int status{ ERR_BAD_ALLOC };
	MediaModelPtr eatonIPMediaAccessorModelPtr{ boost::make_shared<EatonIPCMediaAccessor>() };

	if (eatonIPMediaAccessorModelPtr && 
		ERR_OK == createNewInputPin(NS(pin, 1)::VideoStreamOutputPinID) &&
		ERR_OK == createNewOutputPin(NS(pin, 1)::AudioStreamOutputPinID))
	{
		mediaModelPtr.swap(eatonIPMediaAccessorModelPtr);
		status = MediaFilter::createNewFilter();
	}

	return status;
}

int LivestreamCaptureFilter::destroyFilter()
{
	return ERR_OK;
}

NS_END
