#include "boost/bind.hpp"
#include "boost/make_shared.hpp"
#include "error.h"
#include "MediaPin/InputMediaPin.h"
using InputMediaPin = NS(pin, 1)::InputMediaPin;
#include "MediaPin/OutputMediaPin.h"
using OutputMediaPin = NS(pin, 1)::OutputMediaPin;
#include "MediaFilter/MediaFilter.h"

NS_BEGIN(filter, 1)

MediaFilter::MediaFilter()
{}

MediaFilter::~MediaFilter()
{
	// ======     MOST IMPORTANT     ======
	// Because media pin instance for inputing hold instance of filter using shared_from_this(), 
	// we must clear set of pins before destroying instance of filter.
	mediaPinGroup.clear();
}

int MediaFilter::createNewFilter(void)
{
	if (mediaModelPtr)
	{
		mediaModelPtr->setPostInputMediaDataCallback(
			boost::bind(&MediaFilter::postInputMediaData, boost::enable_shared_from_this<MediaFilter>::shared_from_this(), _1));
	}

	return ERR_OK;
}

int MediaFilter::createNewInputPin(const std::string pinID)
{
	int status{ pinID.empty() ? ERR_INVALID_PARAM : ERR_OK };

	if (ERR_OK == status && !isSourceFilter())
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

	if (ERR_OK == status && !isTargetFilter())
	{
		MediaPinPtr outputPinPtr{ boost::make_shared<OutputMediaPin>() };
		if (outputPinPtr)
		{
			mediaPinGroup.insert(pinID, outputPinPtr);
		}
	}

	return status;
}

int MediaFilter::postInputMediaData(MediaDataPtr mediaData)
{
	int status{ mediaData && !isTargetFilter() ? ERR_OK : ERR_INVALID_PARAM };

	if (ERR_OK == status)
	{
		const MediaDataMainID mediaDataMainID{ mediaData->getMainID() };

		if (MediaDataMainID::MEDIA_DATA_MAIN_ID_VIDEO == mediaDataMainID || MediaDataMainID::MEDIA_DATA_MAIN_ID_IMAGE == mediaDataMainID)
		{
			MediaPinRef mediaPinRef{ queryMediaPinByID(NS(pin, 1)::VideoStreamOutputPinID) };
			if (!mediaPinRef.expired())
			{
				status = mediaPinRef.lock()->inputData(mediaData);
			}
		}
		else if (MediaDataMainID::MEDIA_DATA_MAIN_ID_AUDIO == mediaDataMainID)
		{
			MediaPinRef mediaPinRef{ queryMediaPinByID(NS(pin, 1)::AudioStreamOutputPinID) };
			if (!mediaPinRef.expired())
			{
				status = mediaPinRef.lock()->inputData(mediaData);
			}
		}
		else
		{
			status = ERR_NOT_SUPPORT;
		}
	}

	return status;
}

NS_END
