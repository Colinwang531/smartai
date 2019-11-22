#include "boost/bind.hpp"
#include "boost/make_shared.hpp"
#include "error.h"
#include "MediaModel/Accessor/SDK/HikvisionIPCMediaAccessor.h"
using HikvisionIPCMediaAccessor = NS(model, 1)::HikvisionIPCMediaAccessor;
#include "MediaFilter/Capture/LivestreamCaptureFilter.h"

NS_BEGIN(filter, 1)

LivestreamCaptureFilter::LivestreamCaptureFilter() : SourceMediaFilter()
{}

LivestreamCaptureFilter::~LivestreamCaptureFilter()
{}

int LivestreamCaptureFilter::createNewFilter()
{
	int status{ ERR_BAD_ALLOC };
	MediaModelPtr hikvisionIPMediaAccessorModelPtr{ boost::make_shared<HikvisionIPCMediaAccessor>() };

	if (hikvisionIPMediaAccessorModelPtr &&
		ERR_OK == createNewOutputPin(NS(pin, 1)::VideoStreamOutputPinID) &&
		ERR_OK == createNewOutputPin(NS(pin, 1)::AudioStreamOutputPinID))
	{
		hikvisionIPMediaAccessorModelPtr->setPostInputMediaDataCallback(
			boost::bind(&LivestreamCaptureFilter::postInputMediaData, this, _1));
		mediaModelPtr.swap(hikvisionIPMediaAccessorModelPtr);
		status = MediaFilter::createNewFilter();
	}

	return status;
}

int LivestreamCaptureFilter::destroyFilter()
{
	return ERR_OK;
}

NS_END
