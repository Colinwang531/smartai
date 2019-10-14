#include "error.h"
#include "MediaFilter/MediaFilter.h"

NS_BEGIN(filter, 1)

MediaFilter::MediaFilter()
{}

MediaFilter::~MediaFilter()
{}

int MediaFilter::addPin(const std::string filterID, MediaPinPtr pinPtr)
{
	return filterID.empty() || !pinPtr ? ERR_INVALID_PARAM : mediaPinGroup.insert(filterID, pinPtr);
}

int MediaFilter::removePin(const std::string pinID)
{
	return pinID.empty() ? ERR_INVALID_PARAM : mediaPinGroup.remove(pinID);
}

bool MediaFilter::isSourceFilter() const
{
	return false;
}

bool MediaFilter::isTargetFilter() const
{
	return false;
}

NS_END
