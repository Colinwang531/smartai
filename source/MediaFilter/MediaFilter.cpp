#include "boost/bind.hpp"
#include "boost/make_shared.hpp"
#include "error.h"
#include "MediaData/MediaData.h"
#include "MediaModel/MediaModel.h"
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

		int MediaFilter::createNewFilter(
			const MediaStreamID mediaStreamID /* = MediaStreamID::MEDIA_STREAM_ID_AV */)
		{
			// Filter with empty model is invalid.
			if (mediaModelPtr)
			{
				mediaModelPtr->setPostInputMediaDataCallback(
					boost::bind(
						&MediaFilter::postInputMediaDataCallback, 
						boost::enable_shared_from_this<MediaFilter>::shared_from_this(), 
						_1));
			}

			return ERR_OK;
		}

		int MediaFilter::destroyFilter(void)
		{
			mediaPinGroup.clear();
			return ERR_OK;
		}

		int MediaFilter::inputMediaData(MediaDataPtr mediaData)
		{
			int status{ mediaData ? ERR_OK : ERR_INVALID_PARAM };

			if (ERR_OK == status)
			{
				status = mediaModelPtr ? mediaModelPtr->inputMediaData(mediaData) : postInputMediaDataCallback(mediaData);
			}

			return status;
		}

		int MediaFilter::createNewInputPin(const std::string& pinID)
		{
			int status{ pinID.empty() ? ERR_INVALID_PARAM : ERR_OK };

			if (ERR_OK == status && !isSourceFilter())
			{
				MediaPinPtr inputPinPtr{
					boost::make_shared<InputMediaPin>(
						boost::enable_shared_from_this<MediaFilter>::shared_from_this()) };
				if (inputPinPtr)
				{
					mediaPinGroup.insert(pinID, inputPinPtr);
				}
			}

			return status;
		}

		int MediaFilter::createNewOutputPin(const std::string& pinID)
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

		int MediaFilter::postInputMediaDataCallback(MediaDataPtr mediaData)
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
