#include "boost/make_shared.hpp"
#include "error.h"
#include "MediaPin/InputMediaPin.h"
using InputMediaPin = NS(pin, 1)::InputMediaPin;
#include "MediaPin/OutputMediaPin.h"
using OutputMediaPin = NS(pin, 1)::OutputMediaPin;
#include "MediaFilter/MediaFilter.h"

NS_BEGIN(filter, 1)

MediaFilter::MediaFilter(const MediaFilterMode mode /* = MediaFilterMode::MEDIA_FILTER_MEDIUM */) : mediaFilterMode{ mode }
{}

MediaFilter::~MediaFilter()
{
	// ======     MOST IMPORTANT     ======
	// Because media pin instance for inputing hold instance of filter using shared_from_this(), 
	// we must clear set of pins before destroying instance of filter.
	mediaPinGroup.clear();
}

MediaPinRef MediaFilter::queryMediaPinByID(const std::string pinID)
{
	return mediaPinGroup.at(pinID);
}

int MediaFilter::createNewInputPin(const std::string pinID)
{
	int status{ pinID.empty() ? ERR_INVALID_PARAM : ERR_OK };

	if (ERR_OK == status && 
		(MediaFilterMode::MEDIA_FILTER_MODE_MEDIUM == mediaFilterMode || 
			MediaFilterMode::MEDIA_FILTER_MODE_TARGET == mediaFilterMode))
	{
		MediaPinPtr inputPinPtr{ 
			boost::make_shared<InputMediaPin>(boost::enable_shared_from_this<MediaFilter>::shared_from_this()) };
		if (inputPinPtr)
		{
			mediaPinGroup.insert(pinID, inputPinPtr);
		}
	}

	return status;
}

int MediaFilter::createNewOutputPin(const std::string pinID)
{
	int status{ pinID.empty() ? ERR_INVALID_PARAM : ERR_OK };

	if (ERR_OK == status &&
		(MediaFilterMode::MEDIA_FILTER_MODE_MEDIUM == mediaFilterMode ||
			MediaFilterMode::MEDIA_FILTER_MODE_SOURCE == mediaFilterMode))
	{
		MediaPinPtr outputPinPtr{ boost::make_shared<OutputMediaPin>() };
		if (outputPinPtr)
		{
			mediaPinGroup.insert(pinID, outputPinPtr);
		}
	}

	return status;
}

NS_END
