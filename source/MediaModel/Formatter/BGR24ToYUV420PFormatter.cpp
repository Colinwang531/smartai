#include "boost/make_shared.hpp"
extern "C"
{
#include "libavutil/imgutils.h"
}
#include "error.h"
#include "MediaData/MediaData.h"
using MediaData = NS(media, 1)::MediaData;
using MediaDataPtr = boost::shared_ptr<MediaData>;
#include "MediaModel/Formatter/BGR24ToYUV420PFormatter.h"

NS_BEGIN(model, 1)

BGR24ToYUV420PFormatter::BGR24ToYUV420PFormatter() 
	: MediaModel(), yuv420pImage{ NULL }, yuv420pImageBytes{ 0 }, yuv420pFrame{ NULL }, bgr24Frame{ NULL }, ctx{ NULL }
{}

BGR24ToYUV420PFormatter::~BGR24ToYUV420PFormatter()
{
	deinitialize();
}

int BGR24ToYUV420PFormatter::initialize(
	const unsigned short imageWidth /* = 1920 */, const unsigned short imageHeight /* = 1080 */)
{
	yuv420pImageBytes = av_image_get_buffer_size(AV_PIX_FMT_YUV420P, imageWidth, imageHeight, 1);
	yuv420pImage = reinterpret_cast<unsigned char*>(av_malloc(yuv420pImageBytes));
	yuv420pFrame = av_frame_alloc();
	bgr24Frame = av_frame_alloc();

	if (yuv420pFrame && bgr24Frame && yuv420pImage)
	{
		int ret{ av_image_fill_arrays(
			yuv420pFrame->data, yuv420pFrame->linesize, (const uint8_t*)yuv420pImage, AV_PIX_FMT_YUV420P, imageWidth, imageHeight, 1) };
		if (0 < ret)
		{
			ctx = sws_getContext(
				imageWidth, imageHeight, AV_PIX_FMT_BGR24, imageWidth, imageHeight, AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);
		}
	}

	return ctx ? ERR_OK : ERR_BAD_ALLOC;
}

void BGR24ToYUV420PFormatter::deinitialize()
{
	av_free(yuv420pImage);
	av_frame_free(&yuv420pFrame);
	av_frame_free(&bgr24Frame);
	sws_freeContext(ctx);
}

int BGR24ToYUV420PFormatter::inputMediaData(MediaDataPtr mediaData)
{
	int status{ mediaData ? ERR_OK : ERR_INVALID_PARAM };

	if (ERR_OK == status)
	{
		if (!ctx)
		{
			status = initialize();
		}

		if (ERR_OK == status)
		{
			int ret{ av_image_fill_arrays(
				bgr24Frame->data, bgr24Frame->linesize, mediaData->getData(), AV_PIX_FMT_BGR24, 1920, 1080, 1) };
			if (0 < ret)
			{
				ret = sws_scale(
					ctx, (uint8_t const* const*)bgr24Frame->data, bgr24Frame->linesize, 0, 1080, yuv420pFrame->data, yuv420pFrame->linesize);
				if (0 < ret && postInputMediaDataCallback)
				{
					MediaDataPtr mediaDataPtr{
						boost::make_shared<MediaData>(MediaDataMainID::MEDIA_DATA_MAIN_ID_IMAGE, MediaDataSubID::MEDIA_DATA_SUB_ID_YUV420P) };
					if (mediaDataPtr)
					{
						mediaDataPtr->setData(yuv420pImage, yuv420pImageBytes);
						postInputMediaDataCallback(mediaDataPtr);
					}
				}
			}
		}
	}

	return status;
}

NS_END
