#include "error.h"
#include "MediaFilter/MediaFilter.h"

NS_BEGIN(filter, 1)

MediaFilter::MediaFilter()
{}

MediaFilter::~MediaFilter()
{}

MediaPinPtr MediaFilter::queryPin(const std::string pinID)
{
	MediaPinPtr pinPtr;

	mtx.lock();
	if (!pinID.empty())
	{
		MediaPinGroup::iterator it{ mediaPinGroup.find(pinID) };
		if (mediaPinGroup.end() != it)
		{
			pinPtr = it->second;
		}
	}
	mtx.unlock();

	return pinPtr;
}

int MediaFilter::inputData(MediaDataPtr dataPtr)
{
	int status{ dataPtr ? ERR_OK : ERR_INVALID_PARAM };

	if (ERR_OK == status && !isTargetFilter())
	{
		mtx.lock();
		for (MediaPinGroup::iterator it = mediaPinGroup.begin();
			it != mediaPinGroup.end();
			++it)
		{
			if (it->second->isOutputPin())
			{
				it->second->inputData(dataPtr);
			}
		}
		mtx.unlock();
	}

	return status;
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
