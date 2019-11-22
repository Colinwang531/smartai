#include "boost/bind.hpp"
#include "boost/make_shared.hpp"
#include "error.h"
#include "MediaModel/Encoder/JPEG/YUV420PToJPEGEncoder.h"
using YUV420PToJPEGEncoder = NS(model, 1)::YUV420PToJPEGEncoder;
#include "MediaModel/Formatter/BGR24ToYUV420PFormatter.h"
using BGR24ToYUV420PFormatter = NS(model, 1)::BGR24ToYUV420PFormatter;
using MediaModelPtr = boost::shared_ptr<NS(model, 1)::MediaModel>;
#include "MediaData/MediaData.h"
using MediaData = NS(media, 1)::MediaData;
using MediaDataPtr = boost::shared_ptr<MediaData>;
#include "JpegEncoder.h"

JPEGENCODER_PostJpegEncodeCallback postJpegEncodeCallbak{ NULL };
void* cbUserData{ NULL };
MediaModelPtr yuv420pToJpegEncoderPtr;
MediaModelPtr bgr24ToYuv420pFormatterPtr;

int postBGR24ToYUV420PFormatCallback(MediaDataPtr mediaData)
{
	return mediaData && bgr24ToYuv420pFormatterPtr ? bgr24ToYuv420pFormatterPtr->inputMediaData(mediaData) : ERR_BAD_OPERATE;
}

int postYUV420PToJPEGEncodeCallback(MediaDataPtr mediaData)
{
	if (postJpegEncodeCallbak)
	{
		postJpegEncodeCallbak(mediaData->getData(), mediaData->getDataBytes(), cbUserData);
	}
}

void JPEGENCODER_RegisterPostJpegEncodeCallback(
	JPEGENCODER_PostJpegEncodeCallback callback /* = NULL */, void* userData /* = NULL */)
{
	postJpegEncodeCallbak = callback;
	cbUserData = userData;
}

int JPEGENCODER_EncodeJpegPicture(const unsigned char* data /* = NULL */, const int dataBytes /* = 0 */)
{
	int status{ data && 0 < dataBytes ? ERR_OK : ERR_INVALID_PARAM };

	if (ERR_OK == status)
	{
		if (!bgr24ToYuv420pFormatterPtr)
		{
			MediaModelPtr formatterPtr{ boost::make_shared<BGR24ToYUV420PFormatter>() };
			if (formatterPtr)
			{
				formatterPtr->setPostInputMediaDataCallback(boost::bind(&postBGR24ToYUV420PFormatCallback, _1));
				bgr24ToYuv420pFormatterPtr.swap(formatterPtr);
			}
		}

		if (!yuv420pToJpegEncoderPtr)
		{
			MediaModelPtr encoderPtr{ boost::make_shared<YUV420PToJPEGEncoder>() };
			if (encoderPtr)
			{
				encoderPtr->setPostInputMediaDataCallback(boost::bind(&postYUV420PToJPEGEncodeCallback, _1));
				yuv420pToJpegEncoderPtr.swap(encoderPtr);
			}
		}

		MediaDataPtr mediaDataPtr{ 
			boost::make_shared<MediaData>(MediaDataMainID::MEDIA_DATA_MAIN_ID_IMAGE, MediaDataSubID::MEDIA_DATA_SUB_ID_BGR24) };
		if (mediaDataPtr && bgr24ToYuv420pFormatterPtr)
		{
			mediaDataPtr->setData(data, dataBytes);
			status = bgr24ToYuv420pFormatterPtr->inputMediaData(mediaDataPtr);
		}
		else
		{
			status = ERR_BAD_OPERATE;
		}
	}

	return status;
}
