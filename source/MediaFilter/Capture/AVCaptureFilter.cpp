#include "boost/make_shared.hpp"
#include "error.h"
#include "MediaPin/MediaPin.h"
#include "MediaFilter/Capture/AVCaptureFilter.h"

NS_BEGIN(filter, 1)

AVCaptureFilter::AVCaptureFilter() : TargetMediaFilter(), mediaDataCaptureCallback{ NULL }
{}

AVCaptureFilter::~AVCaptureFilter()
{}

int AVCaptureFilter::createNewMediaCapture(MediaDataCaptureCallback callback /* = NULL */)
{
	createNewInputPin(NS(pin, 1)::MediaStreamInputPinID);
	mediaDataCaptureCallback = callback;
	return ERR_OK;
}

int AVCaptureFilter::inputMediaData(MediaDataPtr mediaData)
{
	return ERR_OK;
}

NS_END
