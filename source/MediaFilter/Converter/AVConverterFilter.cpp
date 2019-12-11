#include "boost/make_shared.hpp"
#include "error.h"
#include "MediaData/MediaData.h"
#include "MediaModule/Converter/Yv12toYuv420pConverter.h"
#include "MediaPin/MediaPin.h"
#include "MediaFilter/Converter/AVConverterFilter.h"

namespace framework
{
	namespace multimedia
	{
		AVConverterFilter::AVConverterFilter() : MediaFilter()
		{}

		AVConverterFilter::~AVConverterFilter()
		{}

		int AVConverterFilter::createNewFilter()
		{
			return ERR_OK == MediaFilter::createNewInputPin(VideoStreamInputPinID) &&
				ERR_OK == MediaFilter::createNewOutputPin(VideoStreamOutputPinID) ? ERR_OK : ERR_BAD_ALLOC;
		}

		int AVConverterFilter::createNewModule(MediaDataPtr mediaData)
		{
			int status{ mediaData ? ERR_OK : ERR_INVALID_PARAM };

			if (ERR_OK == status)
			{
				MediaModulePtr converterModulePtr;
				const MediaDataMainID mediaDataMainID{ mediaData->getMainID() };
				const MediaDataSubID mediaDataSubID{ mediaData->getSubID() };

				if (MediaDataMainID::MEDIA_DATA_MAIN_ID_VIDEO == mediaDataMainID)
				{
					if (MediaDataSubID::MEDIA_DATA_SUB_ID_YV12 == mediaDataSubID)
					{
						converterModulePtr = boost::make_shared<Yv12toYuv420pConverter>();
					}
				}
				else if (MediaDataMainID::MEDIA_DATA_MAIN_ID_AUDIO == mediaDataMainID)
				{
				}

				if (converterModulePtr)
				{
					mediaModulePtr.swap(converterModulePtr);
					MediaFilter::setPostInputMediaDataCallback();
				}
			}

			return status;
		}
	}//namespace multimedia
}//namespace framework
