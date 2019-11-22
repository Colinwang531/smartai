extern "C"
{
#include "libavutil/imgutils.h"
}
#include "error.h"
#include "MediaModel/Encoder/JPEG/YUV420PToJPEGEncoder.h"

NS_BEGIN(model, 1)

YUV420PToJPEGEncoder::YUV420PToJPEGEncoder() : MediaModel(), ctx{ NULL }, yuv420pFrame{ NULL }
{}

YUV420PToJPEGEncoder::~YUV420PToJPEGEncoder()
{}

int YUV420PToJPEGEncoder::initialize(
	const unsigned short imageWidth /* = 1920 */, const unsigned short imageHeight /* = 1080 */)
{
	int status{ ERR_EXISTED };

	if (!codec)
	{
		codec = avcodec_find_encoder(AV_CODEC_ID_MJPEG);

		if (codec && !inputAVFrame)
		{
			inputAVFrame = av_frame_alloc();

			if (inputAVFrame)
			{
				inputAVFrame->format = AV_PIX_FMT_YUV420P;
				inputAVFrame->width = imageWidth;
				inputAVFrame->height = imageHeight;
			} 
			else
			{
				status = ERR_BAD_ALLOC;
			}
		}
		else
		{
			status = ERR_BAD_OPERATE;
		}
	}

	return codec ? FFmpegEncoder::initialize(imageWidth, imageHeight) : status;
}

void YUV420PToJPEGEncoder::deinitialize()
{
	FFmpegEncoder::deinitialize();
	av_frame_free(&inputAVFrame);
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
			yuv420pFrame->data, yuv420pFrame->linesize, mediaData->getData(), AV_PIX_FMT_YUV420P, 1920, 1080, 1) };
		if (0 < ret)
		{
			if (!avcodec_send_frame(ctx, yuv420pFrame))
			{
				AVPacket pkt;
				av_init_packet(&pkt);
				avcodec_receive_packet(ctx, &pkt);
//				status = FFmpegEncoder::encode(pkt.data, pkt.size, imageWidth, imageHeight);
				av_packet_unref(&pkt);
			}
		}
	}
	
	return status;
}

NS_END
