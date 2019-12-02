#include "boost/make_shared.hpp"
#include "error.h"
#include "MediaData/MediaData.h"
// #include "MediaModel/Formatter/YV12ToBGR24Formatter.h"
// using YV12ToBGR24Formatter = NS(model, 1)::YV12ToBGR24Formatter;
#include "MediaFilter/Formatter/ImageFormatterFilter.h"

namespace framework
{
	namespace multimedia
	{
		ImageFormatterFilter::ImageFormatterFilter() : MediaFilter()
		{}

		ImageFormatterFilter::~ImageFormatterFilter()
		{}

		int ImageFormatterFilter::createNewModel(MediaDataPtr mediaData)
		{
			int status{ mediaData ? ERR_OK : ERR_INVALID_PARAM };

			if (ERR_OK == status)
			{
				const MediaDataMainID mediaDataMainID{ mediaData->getMainID() };
				const MediaDataSubID mediaDataSubID{ mediaData->getSubID() };

				if (MediaDataMainID::MEDIA_DATA_MAIN_ID_VIDEO == mediaDataMainID)
				{
					// 				MediaModelPtr videoFormatterPtr{ boost::make_shared<YV12ToBGR24Formatter>() };
// 				if (videoFormatterPtr)
// 				{
// 					mediaModelPtr.swap(videoFormatterPtr);
// 					status = ERR_OK;
// 				}
				}
				if (MediaDataMainID::MEDIA_DATA_MAIN_ID_AUDIO == mediaDataMainID)
				{
				}
			}

			return ERR_OK == status ? MediaFilter::createNewModel(mediaData) : status;
		}
	}//namespace multimedia
}//namespace framework
