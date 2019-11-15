extern "C"
{
#include "libavutil/imgutils.h"
}
#include "error.h"
#include "MediaEncoder/JPEG/YUV420PToJPEGEncoder.h"

NS_BEGIN(encoder, 1)

YUV420PToJPEGEncoder::YUV420PToJPEGEncoder() : FFmpegEncoder(), inputAVFrame{ NULL }
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

int YUV420PToJPEGEncoder::encode(
	const unsigned char* imageData /* = NULL */, const unsigned long long imageBytes /* = 0 */, 
	const unsigned short imageWidth /* = 1920 */, const unsigned short imageHeight /* = 1080 */)
{
	int status{ ERR_INVALID_PARAM };

	if(inputAVFrame)
	{
		av_image_fill_arrays(
			inputAVFrame->data, inputAVFrame->linesize, (const uint8_t*)imageData, 
			AV_PIX_FMT_YUV420P, imageWidth, imageHeight, 1);

		if(!avcodec_send_frame(ctx, inputAVFrame))
		{
			AVPacket pkt;
			av_init_packet(&pkt);
			avcodec_receive_packet(ctx, &pkt);
			status = FFmpegEncoder::encode(pkt.data, pkt.size, imageWidth, imageHeight);
			av_packet_unref(&pkt);
		}
		else
		{
			status = ERR_BAD_OPERATE;
		}
	}
	
	return status;
}

NS_END
