#include "boost/make_shared.hpp"
#include "error.h"
#include "MediaModel/Formatter/YV12ToBGR24Formatter.h"
using YV12ToBGR24Formatter = NS(model, 1)::YV12ToBGR24Formatter;
#include "MediaFilter/Formatter/VideoFormatterFilter.h"

NS_BEGIN(filter, 1)

VideoFormatterFilter::VideoFormatterFilter() : MediaFilter()
{}

VideoFormatterFilter::~VideoFormatterFilter()
{}

int VideoFormatterFilter::createNewFilter(void)
{
	int status{ ERR_BAD_ALLOC };

	if (ERR_OK == createNewInputPin(NS(pin, 1)::VideoStreamInputPinID) && 
		ERR_OK == createNewOutputPin(NS(pin, 1)::VideoStreamOutputPinID))
	{
		MediaModelPtr videoFormatterPtr{ boost::make_shared<YV12ToBGR24Formatter>() };
		if (videoFormatterPtr)
		{
			mediaModelPtr.swap(videoFormatterPtr);
			status = ERR_OK;
		}
	}

	return ERR_OK == status ? MediaFilter::createNewFilter() : status;
}

int VideoFormatterFilter::destroyFilter()
{
	return ERR_OK;
}

int VideoFormatterFilter::inputMediaData(MediaDataPtr mediaData)
{
	return mediaData && mediaModelPtr ? mediaModelPtr->inputMediaData(mediaData) : ERR_INVALID_PARAM;
}

NS_END
