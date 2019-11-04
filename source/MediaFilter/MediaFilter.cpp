#include "error.h"
#include "MediaFilter/MediaFilter.h"

NS_BEGIN(filter, 1)

MediaFilter::MediaFilter(const MediaFilterMode mode /* = MediaFilterMode::MEDIA_FILTER_MEDIUM */) : mediaFilterMode{ mode }
{}

MediaFilter::~MediaFilter()
{}

MediaPinRef MediaFilter::queryMediaPinByID(const std::string pinID)
{
	return mediaPinGroup.at(pinID);
}

NS_END
