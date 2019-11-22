#include "boost/make_shared.hpp"
extern "C"
{
#include "libavutil/imgutils.h"
}
#include "error.h"
#include "MediaModel/Formatter/YV12ToBGR24Formatter.h"

NS_BEGIN(model, 1)

YV12ToBGR24Formatter::YV12ToBGR24Formatter()
	: MediaModel(), yuv420pImage{ NULL }, bgr24Image{ NULL },
	yuv420pFrame{ NULL }, bgr24Frame{ NULL }, ctx{ NULL },
	yuv420pImageBytes{ 0 }, bgr24ImageBytes{ 0 }
{}

YV12ToBGR24Formatter::~YV12ToBGR24Formatter()
{
	deinitialize();
}

int YV12ToBGR24Formatter::initialize(
	const unsigned short imageWidth /* = 1920 */, const unsigned short imageHeight /* = 1080 */)
{
	yuv420pImageBytes = av_image_get_buffer_size(AV_PIX_FMT_YUV420P, imageWidth, imageHeight, 1);
	bgr24ImageBytes = av_image_get_buffer_size(AV_PIX_FMT_BGR24, imageWidth, imageHeight, 1);
	yuv420pImage = reinterpret_cast<unsigned char*>(av_malloc(yuv420pImageBytes));
	bgr24Image = reinterpret_cast<unsigned char*>(av_malloc(bgr24ImageBytes));
	yuv420pFrame = av_frame_alloc();
	bgr24Frame = av_frame_alloc();
	ctx = sws_getContext(
		imageWidth, imageHeight, AV_PIX_FMT_YUV420P, imageWidth, imageHeight, AV_PIX_FMT_BGR24, SWS_BICUBIC, NULL, NULL, NULL);

	if (yuv420pFrame && yuv420pImage)
	{
		av_image_fill_arrays(
			yuv420pFrame->data, yuv420pFrame->linesize, (const uint8_t*)yuv420pImage, AV_PIX_FMT_YUV420P, imageWidth, imageHeight, 1);
		av_image_fill_arrays(
			bgr24Frame->data, bgr24Frame->linesize, (const uint8_t*)bgr24Image, AV_PIX_FMT_BGR24, imageWidth, imageHeight, 1);
	}

	return yuv420pImage && bgr24Image && yuv420pFrame && bgr24Frame && ctx? ERR_OK : ERR_BAD_ALLOC;
}

void YV12ToBGR24Formatter::deinitialize()
{
	av_free(yuv420pImage);
	av_free(bgr24Image);
	av_frame_free(&yuv420pFrame);
	av_frame_free(&bgr24Frame);
	sws_freeContext(ctx);
}

int YV12ToBGR24Formatter::inputMediaData(MediaDataPtr mediaData)
{
	int status{ ERR_INVALID_PARAM };
	const unsigned char* data{ mediaData->getData() };
	const unsigned long long dataBytes{ mediaData->getDataBytes() };
	const int imageWidth{ mediaData->getWidth() }, imageHeight{ mediaData->getHeight() };

	if (data && 0 < dataBytes && 0 < imageWidth && 0 < imageHeight)
	{
		if (!ctx)
		{
			initialize(imageWidth, imageHeight);
		}

		int yStrideBytes = imageWidth * imageHeight;
		int uvStrideBytes = yStrideBytes / 4;
		int srcUOffset = yStrideBytes;
		int srcVOffset = srcUOffset + uvStrideBytes;
		int dstVOffset = yStrideBytes;
		int dstUOffset = dstVOffset + uvStrideBytes;

#ifdef _WINDOWS
		memcpy_s(yuv420pImage, yStrideBytes, data, yStrideBytes);
		memcpy_s(yuv420pImage + dstVOffset, uvStrideBytes, data + srcVOffset, uvStrideBytes);
		memcpy_s(yuv420pImage + dstUOffset, uvStrideBytes, data + srcUOffset, uvStrideBytes);
#else
		memcpy(yuv420pImage, imageData, yStrideBytes);
		memcpy(yuv420pImage + dstVOffset, data + srcVOffset, uvStrideBytes);
		memcpy(yuv420pImage + dstUOffset, data + srcUOffset, uvStrideBytes);
#endif//_WINDOWS

		int height{ sws_scale(
			ctx, (uint8_t const* const*)yuv420pFrame->data, yuv420pFrame->linesize, 0, imageHeight, bgr24Frame->data, bgr24Frame->linesize) };
		if (0 < height && postInputMediaDataCallback)
		{
			MediaDataPtr rgb24MediaData{
			boost::make_shared<NS(media, 1)::MediaData>(
				MediaDataMainID::MEDIA_DATA_MAIN_ID_IMAGE, MediaDataSubID::MEDIA_DATA_SUB_ID_BGR24) };
			if (rgb24MediaData)
			{
				rgb24MediaData->setData(bgr24Image, bgr24ImageBytes);
				rgb24MediaData->setPixel(imageWidth, imageHeight);
				postInputMediaDataCallback(rgb24MediaData);
				status = ERR_OK;
			}
		}
	}

 	return status;
}

NS_END
