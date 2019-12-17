#include "boost/bind.hpp"
#include "boost/make_shared.hpp"
#include "error.h"
//#include "MediaData/MediaData.h"
//#include "MediaModule/MediaModule.h"
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

		int MediaFilter::createNewFilter(const std::string filterID)
		{
			int status{ filterID.empty() ? ERR_INVALID_PARAM : ERR_OK };

			if (ERR_OK == status)
			{
				MediaPinPtr mediaPinPtr{ queryMediaPinByID(filterID) };

				if (mediaPinPtr)
				{
					status = ERR_EXISTED;
				}
				else
				{
					if (!filterID.compare(AVCaptureFilterID))
					{
						createNewOutputPin(VideoStreamOutputPinID);
						createNewOutputPin(AudioStreamOutputPinID);
					}
					else if (!filterID.compare(AVParserFilterID) || !filterID.compare(AVControllerFilterID))
					{
						createNewInputPin(VideoStreamInputPinID);
						createNewInputPin(AudioStreamInputPinID);
						createNewOutputPin(VideoStreamOutputPinID);
						createNewOutputPin(AudioStreamOutputPinID);
					}
					else if (!filterID.compare(VideoDecoderFilterID) || !filterID.compare(ImageConverterFilterID) || !filterID.compare(VideoRendererFilterID))
					{
						createNewInputPin(VideoStreamInputPinID);
						createNewOutputPin(VideoStreamOutputPinID);
					}
					else if (!filterID.compare(AudioDecoderFilterID) || !filterID.compare(SoundPlayerFilterID))
					{
						createNewInputPin(AudioStreamInputPinID);
						createNewOutputPin(AudioStreamOutputPinID);
					}
					else if (!filterID.compare(AVCallbackFilterID))
					{
						createNewInputPin(VideoStreamInputPinID);
						createNewInputPin(AudioStreamInputPinID);
					}
					else
					{
						status = ERR_NOT_SUPPORT;
					}
				}
			}

			return status;
		}

		int MediaFilter::destroyFilter()
		{
			mediaPinGroup.clear();
			return ERR_OK;
		}

// 		int MediaFilter::inputMediaData(MediaDataPtr mediaData)
// 		{
// 			int status{ mediaData ? ERR_OK : ERR_INVALID_PARAM };
// 
// 			if (ERR_OK == status)
// 			{
// 				if (!mediaModulePtr)
// 				{
// 					createNewModule(mediaData);
// 				}
// 
// 				// Media data is passed directly to next filter if model in the filter is not exist.
// 				status = mediaModulePtr ? mediaModulePtr->inputMediaData(mediaData) : postInputMediaDataCallback(mediaData);
// 			}
// 
// 			return status;
// 		}
// 
// 		void MediaFilter::setPostInputMediaDataCallback()
// 		{
// 			if (mediaModulePtr)
// 			{
// 				mediaModulePtr->setPostInputMediaDataCallback(
// 					boost::bind(&MediaFilter::postInputMediaDataCallback, boost::enable_shared_from_this<MediaFilter>::shared_from_this(), _1));
// 			}
// 		}
// 
// 		int MediaFilter::postInputMediaDataCallback(MediaDataPtr mediaData)
// 		{
// 			int status{ mediaData && !isTargetFilter() ? ERR_OK : ERR_INVALID_PARAM };
// 
// 			if (ERR_OK == status)
// 			{
// 				const MediaDataMainID mediaDataMainID{ mediaData->getMainID() };
// 
// 				if (MediaDataMainID::MEDIA_DATA_MAIN_ID_VIDEO == mediaDataMainID || MediaDataMainID::MEDIA_DATA_MAIN_ID_IMAGE == mediaDataMainID)
// 				{
// 					MediaPinRef mediaPinRef{ queryMediaPinByID(VideoStreamOutputPinID) };
// 					if (!mediaPinRef.expired())
// 					{
// 						status = mediaPinRef.lock()->inputMediaData(mediaData);
// 					}
// 				}
// 				else if (MediaDataMainID::MEDIA_DATA_MAIN_ID_AUDIO == mediaDataMainID)
// 				{
// 					MediaPinRef mediaPinRef{ queryMediaPinByID(AudioStreamOutputPinID) };
// 					if (!mediaPinRef.expired())
// 					{
// 						status = mediaPinRef.lock()->inputMediaData(mediaData);
// 					}
// 				}
// 				else
// 				{
// 					status = ERR_NOT_SUPPORT;
// 				}
// 			}
// 
// 			return status;
// 		}

		int MediaFilter::createNewInputPin(const std::string pinID)
		{
			int status{ pinID.empty() ? ERR_INVALID_PARAM : ERR_OK };

			if (ERR_OK == status)
			{
				MediaPinPtr mediaPinPtr{
					boost::make_shared<InputMediaPin>(
						boost::enable_shared_from_this<MediaFilter>::shared_from_this()) };
				if (mediaPinPtr)
				{
					mediaPinGroup.insert(pinID, mediaPinPtr);
				}
				else
				{
					status = ERR_BAD_ALLOC;
				}
			}

			return status;
		}

		int MediaFilter::createNewOutputPin(const std::string pinID)
		{
			int status{ pinID.empty() ? ERR_INVALID_PARAM : ERR_OK };

			if (ERR_OK == status)
			{
				MediaPinPtr mediaPinPtr{ boost::make_shared<OutputMediaPin>() };
				if (mediaPinPtr)
				{
					mediaPinGroup.insert(pinID, mediaPinPtr);
				}
				else
				{
					status = ERR_BAD_ALLOC;
				}
			}

			return status;
		}
	}//namespace multimedia
}//namespace framework
