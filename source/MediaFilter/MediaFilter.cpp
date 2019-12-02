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
			if (!isSourceFilter())
			{
				createNewInputPin(mediaStreamID);
			}

			if (!isTargetFilter())
			{
				createNewOutputPin(mediaStreamID);
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
				if (!mediaModelPtr)
				{
					status = createNewModel(mediaData);
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

		int MediaFilter::createNewInputPin(
			const MediaStreamID mediaStreamID /* = MediaStreamID::MEDIA_STREAM_ID_AV */)
		{
			if (MediaStreamID::MEDIA_STREAM_ID_AV == mediaStreamID || MediaStreamID::MEDIA_STREAM_ID_VIDEO == mediaStreamID)
			{
				MediaPinPtr inputPinPtr{
					boost::make_shared<InputMediaPin>(
						boost::enable_shared_from_this<MediaFilter>::shared_from_this()) };
				if (inputPinPtr)
				{
					mediaPinGroup.insert(VideoStreamInputPinID, inputPinPtr);
				}
			}
			if (MediaStreamID::MEDIA_STREAM_ID_AV == mediaStreamID || MediaStreamID::MEDIA_STREAM_ID_AUDIO == mediaStreamID)
			{
				MediaPinPtr inputPinPtr{
					boost::make_shared<InputMediaPin>(
						boost::enable_shared_from_this<MediaFilter>::shared_from_this()) };
				if (inputPinPtr)
				{
					mediaPinGroup.insert(AudioStreamInputPinID, inputPinPtr);
				}
			}

			return ERR_OK;
		}

		int MediaFilter::createNewOutputPin(
			const MediaStreamID mediaStreamID /* = MediaStreamID::MEDIA_STREAM_ID_AV */)
		{
			if (MediaStreamID::MEDIA_STREAM_ID_AV == mediaStreamID || MediaStreamID::MEDIA_STREAM_ID_VIDEO == mediaStreamID)
			{
				MediaPinPtr outputPinPtr{ boost::make_shared<OutputMediaPin>() };
				if (outputPinPtr)
				{
					mediaPinGroup.insert(VideoStreamOutputPinID, outputPinPtr);
				}
			}
			if (MediaStreamID::MEDIA_STREAM_ID_AV == mediaStreamID || MediaStreamID::MEDIA_STREAM_ID_AUDIO == mediaStreamID)
			{
				MediaPinPtr outputPinPtr{ boost::make_shared<OutputMediaPin>() };
				if (outputPinPtr)
				{
					mediaPinGroup.insert(AudioStreamOutputPinID, outputPinPtr);
				}
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
