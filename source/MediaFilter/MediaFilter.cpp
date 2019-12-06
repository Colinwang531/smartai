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
			const unsigned char iflag /* = 0 */, const unsigned char oflag /* = 0 */)
		{
			if (!isSourceFilter())
			{
				createNewInputPin(iflag);
			}

			if (!isTargetFilter())
			{
				createNewOutputPin(oflag);
			}

			return ERR_OK;
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
				if (!mediaModelPtr)
				{
					createNewModel(mediaData);
				}

				// Media data is passed directly to next filter if model in the filter is not exist.
				status = mediaModelPtr ? mediaModelPtr->inputMediaData(mediaData) : postInputMediaData(mediaData);
			}

			return status;
		}

		int MediaFilter::createNewModel(MediaDataPtr mediaData)
		{
			if (mediaModelPtr)
			{
				mediaModelPtr->setPostInputMediaDataCallback(
					boost::bind(
						&MediaFilter::postInputMediaData,
						boost::enable_shared_from_this<MediaFilter>::shared_from_this(),
						_1));
			}

			return ERR_OK;
		}

		int MediaFilter::createNewInputPin(const unsigned char iflag /* = 0 */)
		{
			const unsigned char videoflag{ iflag & 1 }, audioflag{ (iflag >> 1) & 1 };
			MediaPinPtr videoInputPinPtr{
					boost::make_shared<InputMediaPin>(
						boost::enable_shared_from_this<MediaFilter>::shared_from_this()) };
			MediaPinPtr audioInputPinPtr{
					boost::make_shared<InputMediaPin>(
						boost::enable_shared_from_this<MediaFilter>::shared_from_this()) };

			if (0 < videoflag && videoInputPinPtr)
			{
				mediaPinGroup.insert(VideoStreamInputPinID, videoInputPinPtr);
			}
			if (0 < audioflag && audioInputPinPtr)
			{
				mediaPinGroup.insert(AudioStreamInputPinID, audioInputPinPtr);
			}

			return ERR_OK;
		}

		int MediaFilter::createNewOutputPin(const unsigned char oflag /* = 0 */)
		{
			const unsigned char videoflag{ oflag & 1 }, audioflag{ (oflag >> 1) & 1 };
			MediaPinPtr videoOutputPinPtr{ boost::make_shared<OutputMediaPin>() };
			MediaPinPtr audioOutputPinPtr{ boost::make_shared<OutputMediaPin>() };

			if (0 < videoflag && videoOutputPinPtr)
			{
				mediaPinGroup.insert(VideoStreamOutputPinID, videoOutputPinPtr);
			}
			if (0 < audioflag && audioOutputPinPtr)
			{
				mediaPinGroup.insert(AudioStreamOutputPinID, audioOutputPinPtr);
			}

			return ERR_OK;
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
