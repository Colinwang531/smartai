#include "boost/make_shared.hpp"
#include "error.h"
#include "MediaFilter/Capture/AVCaptureFilter.h"

NS_BEGIN(filter, 1)

AVCaptureFilter::AVCaptureFilter() : TargetMediaFilter()
{}

AVCaptureFilter::~AVCaptureFilter()
{}

int AVCaptureFilter::createNewFilter()
{
	int status{ ERR_BAD_ALLOC };

	if (ERR_OK == createNewInputPin(NS(pin, 1)::VideoStreamInputPinID) &&
		ERR_OK == createNewOutputPin(NS(pin, 1)::AudioStreamInputPinID))
	{
		status = ERR_OK;
	}

	return status;
}

int AVCaptureFilter::destroyFilter()
{
	return ERR_OK;
}

int AVCaptureFilter::inputMediaData(MediaDataPtr mediaData)
{
	return ERR_OK;
}

NS_END
