#include "boost/bind.hpp"
#include "boost/make_shared.hpp"
#include "error.h"
#include "MediaData/MediaData.h"
#include "MediaModule/MediaModule.h"
#include "MediaPin/InputMediaPin.h"
#include "MediaPin/OutputMediaPin.h"
#include "MediaFilter/MediaFilter.h"

namespace framework
{
	namespace multimedia
	{
		MediaFilter::MediaFilter()
		{}

		MediaFilter::~MediaFilter()
		{
			// ======     MOST IMPORTANT     ======
			// Because media pin instance for inputing hold instance of filter using shared_from_this(), 
			// we must clear set of pins before destroying instance of filter.
			destroyFilter();
		}

		int MediaFilter::destroyFilter()
		{
			mediaPinGroup.clear();
			return ERR_OK;
		}

		int MediaFilter::inputMediaData(MediaDataPtr mediaData)
		{
			int status{ mediaData ? ERR_OK : ERR_INVALID_PARAM };

			if (ERR_OK == status)
			{
				if (!mediaModulePtr)
				{
					createNewModule(mediaData);
				}

				// Media data is passed directly to next filter if model in the filter is not exist.
				status = mediaModulePtr ? mediaModulePtr->inputMediaData(mediaData) : postInputMediaData(mediaData);
			}

			return status;
		}

		int MediaFilter::createNewModule(MediaDataPtr mediaData)
		{
			if (mediaModulePtr)
			{
				mediaModulePtr->setPostInputMediaDataCallback(
					boost::bind(
						&MediaFilter::postInputMediaData,
						boost::enable_shared_from_this<MediaFilter>::shared_from_this(),
						_1));
			}

			return ERR_OK;
		}

		int MediaFilter::createNewInputPin(const std::string& pinID)
		{
			int status{ ERR_BAD_ALLOC };

			if (!pinID.empty())
			{
				MediaPinPtr mediaPinPtr{
					boost::make_shared<InputMediaPin>(
						boost::enable_shared_from_this<MediaFilter>::shared_from_this()) };
				if (mediaPinPtr)
				{
					mediaPinGroup.insert(pinID, mediaPinPtr);
					status = ERR_OK;
				}
			}

			return status;
		}

		int MediaFilter::createNewOutputPin(const std::string& pinID)
		{
			int status{ ERR_BAD_ALLOC };

			if (!pinID.empty())
			{
				MediaPinPtr mediaPinPtr{ boost::make_shared<OutputMediaPin>() };
				if (mediaPinPtr)
				{
					mediaPinGroup.insert(pinID, mediaPinPtr);
					status = ERR_OK;
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
					MediaPinRef mediaPinRef{ queryMediaPinByID(VideoStreamOutputPinID) };
					if (!mediaPinRef.expired())
					{
						status = mediaPinRef.lock()->inputMediaData(mediaData);
					}
				}
				else if (MediaDataMainID::MEDIA_DATA_MAIN_ID_AUDIO == mediaDataMainID)
				{
					MediaPinRef mediaPinRef{ queryMediaPinByID(AudioStreamOutputPinID) };
					if (!mediaPinRef.expired())
					{
						status = mediaPinRef.lock()->inputMediaData(mediaData);
					}
				}
				else
				{
					status = ERR_NOT_SUPPORT;
				}
			}

			return status;
		}
	}//namespace multimedia
}//namespace framework
