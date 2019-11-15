#include "boost/make_shared.hpp"
#include "error.h"
#include "MediaPin/MediaPin.h"
#include "MediaFilter/Capture/AVCaptureFilter.h"

NS_BEGIN(filter, 1)

AVCaptureFilter::AVCaptureFilter(void* callback /* = NULL */)
	: TargetMediaFilter(), avMediaDataCaptureCallback{ reinterpret_cast<AVMediaDataCaptureCallback>(callback) }
{}

AVCaptureFilter::~AVCaptureFilter()
{}

int AVCaptureFilter::createNewFilter()
{
	return createNewInputPin(NS(pin, 1)::MediaStreamInputPinID);
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
