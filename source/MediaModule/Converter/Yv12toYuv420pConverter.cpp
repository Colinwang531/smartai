extern "C"
{
#include "libavutil/imgutils.h"
}
#include "boost/checked_delete.hpp"
#include "boost/make_shared.hpp"
#include "error.h"
#include "MediaData/MediaData.h"
#include "MediaModule/Converter/Yv12toYuv420pConverter.h"

namespace framework
{
	namespace multimedia
	{
		Yv12toYuv420pConverter::Yv12toYuv420pConverter() : MediaConverter()
		{}

		Yv12toYuv420pConverter::~Yv12toYuv420pConverter()
		{}

		int Yv12toYuv420pConverter::inputMediaData(MediaDataPtr mediaData)
		{
			int status{ mediaData ? ERR_OK : ERR_INVALID_PARAM };

			if (ERR_OK == status)
			{
				int width{ 0 }, height{ 0 };
				mediaData->getImageParameter(width, height);
				const int outputDataBytes{
					av_image_get_buffer_size(AV_PIX_FMT_YUV420P, width, height, 1) };
				unsigned char* outputData = reinterpret_cast<unsigned char*>(av_malloc(outputDataBytes));

				if (outputData && 0 < outputDataBytes)
				{
					int yStrideBytes = width * height;
					int uvStrideBytes = yStrideBytes / 4;
					int srcUOffset = yStrideBytes;
					int srcVOffset = srcUOffset + uvStrideBytes;
					int dstVOffset = yStrideBytes;
					int dstUOffset = dstVOffset + uvStrideBytes;
					const unsigned char* imageData{ mediaData->getData() };

#ifdef _WINDOWS
					memcpy_s(outputData, yStrideBytes, imageData, yStrideBytes);
					memcpy_s(outputData + dstVOffset, uvStrideBytes, imageData + srcVOffset, uvStrideBytes);
					memcpy_s(outputData + dstUOffset, uvStrideBytes, imageData + srcUOffset, uvStrideBytes);
#else
					memcpy(outputData, imageData, yStrideBytes);
					memcpy(outputData + dstVOffset, imageData + srcVOffset, uvStrideBytes);
					memcpy(outputData + dstUOffset, imageData + srcUOffset, uvStrideBytes);
#endif//_WINDOWS
				}

				MediaDataPtr mediaDataPtr{ 
					boost::make_shared<MediaData>(
						MediaDataMainID::MEDIA_DATA_MAIN_ID_VIDEO, MediaDataSubID::MEDIA_DATA_SUB_ID_YUV420P) };
				if (mediaData && mediaDataPtr && outputData && 0 < outputDataBytes)
				{
					mediaDataPtr->setData(outputData, outputDataBytes, true);
					mediaDataPtr.swap(mediaData);
				}
			}

			return status;
		}
	}//namespace multimedia
}//namespace framework
