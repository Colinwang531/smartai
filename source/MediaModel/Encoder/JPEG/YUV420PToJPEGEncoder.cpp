#include "boost/make_shared.hpp"
extern "C"
{
#include "libavutil/imgutils.h"
}
#include "error.h"
#include "MediaData/MediaData.h"
using MediaData = NS(media, 1)::MediaData;
using MediaDataPtr = boost::shared_ptr<MediaData>;
#include "MediaModel/Encoder/JPEG/YUV420PToJPEGEncoder.h"

NS_BEGIN(model, 1)

YUV420PToJPEGEncoder::YUV420PToJPEGEncoder() : MediaModel(), ctx{ NULL }, yuv420pFrame{ NULL }
{}

YUV420PToJPEGEncoder::~YUV420PToJPEGEncoder()
{}

int YUV420PToJPEGEncoder::initialize(
	const unsigned short imageWidth /* = 1920 */, const unsigned short imageHeight /* = 1080 */)
{
	int status{ 0 < imageWidth && 0 < imageHeight ? ERR_OK : ERR_INVALID_PARAM };

	if (ERR_OK == status && !ctx)
	{
		status = ERR_BAD_ALLOC;
		AVCodec* codec{ avcodec_find_encoder(AV_CODEC_ID_MJPEG) };
		if (codec)
		{
			yuv420pFrame = av_frame_alloc();
			ctx = avcodec_alloc_context3(codec);
			ctx->pix_fmt = AV_PIX_FMT_YUVJ420P;
			ctx->codec_type = AVMEDIA_TYPE_VIDEO;
			ctx->time_base.num = 1;
			ctx->time_base.den = 25;
			ctx->width = imageWidth;
			ctx->height = imageHeight;

			int ret{ avcodec_open2(ctx, codec, NULL) };
			status = !ret ? ERR_OK : ERR_BAD_OPERATE;
		}
	}

	return status;
}

void YUV420PToJPEGEncoder::deinitialize()
{
	av_frame_free(&yuv420pFrame);
	avcodec_close(ctx);
}

int YUV420PToJPEGEncoder::inputMediaData(MediaDataPtr mediaData)
{
	int status{ mediaData ? ERR_OK : ERR_INVALID_PARAM };

	if(ERR_OK == status)
	{
		if (!ctx)
		{
			initialize();
		}

		int ret{ av_image_fill_arrays(
			yuv420pFrame->data, yuv420pFrame->linesize, reinterpret_cast<const uint8_t*>(mediaData->getData()), AV_PIX_FMT_YUV420P, 1920, 1080, 1) };
		if (0 < ret)
		{
			if (!avcodec_send_frame(ctx, yuv420pFrame))
			{
				AVPacket pkt;
				av_init_packet(&pkt);
				avcodec_receive_packet(ctx, &pkt);

				MediaDataPtr mediaDataPtr{
						boost::make_shared<MediaData>(MediaDataMainID::MEDIA_DATA_MAIN_ID_IMAGE, MediaDataSubID::MEDIA_DATA_SUB_ID_JPEG) };
				if (postInputMediaDataCallback)
				{
					
					if (mediaDataPtr)
					{
						mediaDataPtr->setData(pkt.data, pkt.size);
						mediaDataPtr->setPixel(1920, 1080);
						postInputMediaDataCallback(mediaDataPtr);
					}
				}
				av_packet_unref(&pkt);
			}
		}
	}
	
	return status;
}

NS_END
