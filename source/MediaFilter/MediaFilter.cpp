#include "error.h"
#include "MediaFilter/MediaFilter.h"

NS_BEGIN(filter, 1)

MediaFilter::MediaFilter(const MediaFilterMode mode /* = MediaFilterMode::MEDIA_FILTER_MEDIUM */) : mediaFilterMode{ mode }
{}

MediaFilter::~MediaFilter()
{}

const MediaPinRef& MediaFilter::queryPinByID(const std::string pinID)
{
	return mediaPinGroup.at(pinID);
}

int MediaFilter::addPin(const std::string pinID, MediaPinPtr pinPtr)
{
	int status{ !pinID.empty() && pinPtr ? ERR_OK : ERR_INVALID_PARAM };

	if (ERR_OK == status)
	{
		mediaPinGroup.insert(pinID, pinPtr);
	}

	return status;
}

int MediaFilter::removePin(const std::string pinID)
{
	int status{ pinID.empty() ? ERR_INVALID_PARAM : ERR_OK };

	if (ERR_OK == status)
	{
		mediaPinGroup.remove(pinID);
	}

	return status;
}

int MediaFilter::inputData(MediaDataPtr dataPtr)
{
	int status{ dataPtr ? ERR_OK : ERR_INVALID_PARAM };

	if (ERR_OK == status)
	{
		std::vector<MediaPinPtr> outputPinPtrs;
		mediaPinGroup.values(outputPinPtrs);

		for (int i = 0; i != outputPinPtrs.size(); ++i)
		{
			if (NS(pin, 1)::MediaPinMode::MEDIA_PIN_OUTPUT == outputPinPtrs[i]->getMode())
			{
				outputPinPtrs[i]->inputData(dataPtr);
			}
		}
	}

	return status;
}

NS_END
