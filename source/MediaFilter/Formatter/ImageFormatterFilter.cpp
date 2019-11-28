#include "boost/make_shared.hpp"
#include "error.h"
#include "MediaPin/MediaPin.h"
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

		int ImageFormatterFilter::createNewFilter(
			const MediaStreamID mediaStreamID /* = MediaStreamID::MEDIA_STREAM_ID_AV */)
		{
			int status{ ERR_BAD_ALLOC };

			if (MediaStreamID::MEDIA_STREAM_ID_AV == mediaStreamID || MediaStreamID::MEDIA_STREAM_ID_VIDEO == mediaStreamID)
			{
				createNewInputPin(VideoStreamInputPinID);
				createNewOutputPin(VideoStreamOutputPinID);
// 				MediaModelPtr videoFormatterPtr{ boost::make_shared<YV12ToBGR24Formatter>() };
// 				if (videoFormatterPtr)
// 				{
// 					mediaModelPtr.swap(videoFormatterPtr);
// 					status = ERR_OK;
// 				}
			}

			return MediaFilter::createNewFilter(mediaStreamID);
		}

		int ImageFormatterFilter::inputMediaData(MediaDataPtr mediaData)
		{
			return ERR_OK;
//			return mediaData && mediaModelPtr ? mediaModelPtr->inputMediaData(mediaData) : ERR_INVALID_PARAM;
		}
	}//namespace multimedia
}//namespace framework
