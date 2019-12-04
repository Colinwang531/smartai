#include "boost/make_shared.hpp"
#include "error.h"
#include "MediaData/MediaData.h"
#include "MediaModel/Formatter/FFmpeg/FFmpegImageFormatter.h"
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
					MediaModelPtr imageFormatterPtr{ 
						boost::make_shared<FFmpegImageFormatter>(MediaDataSubID::MEDIA_DATA_SUB_ID_YUV420P) };
					if (imageFormatterPtr)
					{
						mediaModelPtr.swap(imageFormatterPtr);
					}
				}
				if (MediaDataMainID::MEDIA_DATA_MAIN_ID_AUDIO == mediaDataMainID)
				{
				}
			}

			return ERR_OK == status ? MediaFilter::createNewModel(mediaData) : status;
		}
	}//namespace multimedia
}//namespace framework
