extern "C"
{
#include "libavutil/imgutils.h"
}
#include "error.h"
#include "MediaEncoder/FFmpeg/YV12ToJPEG.h"

NS_BEGIN(encoder, 1)

YV12ToJPEG::YV12ToJPEG()
	: MediaEncoder(), yuv420PFrameData{ NULL }, avCodec{ NULL }, avCodecCtx{ NULL }, openAVCodec{ false }
{
	avCodec = avcodec_find_encoder(AV_CODEC_ID_MJPEG);
	avCodecCtx = avcodec_alloc_context3(avCodec);
	avCodecCtx->pix_fmt = AV_PIX_FMT_YUVJ420P;
	avCodecCtx->codec_type = AVMEDIA_TYPE_VIDEO;
	avCodecCtx->time_base.num = 1;
	avCodecCtx->time_base.den = 11;
//	avcodec_open2(avCodecCtx, avCodec, NULL);
}

YV12ToJPEG::~YV12ToJPEG()
{
	avcodec_close(avCodecCtx);
	av_free(avCodec);
	av_free(yuv420PFrameData);
}

int YV12ToJPEG::encode(char*& jpegData, int& jpegBytes,
	const char* data /* = NULL */, const int dataBytes /* = 0 */, const int w /* = 1920 */, const int h /* = 1080 */)
{
	int status{ MediaEncoder::encode(jpegData, jpegBytes, data, dataBytes, w, h) };

	if (ERR_OK == status && avCodec && avCodecCtx)
	{
		if (!openAVCodec)
		{
			avCodecCtx->width = w;
			avCodecCtx->height = h;
			int ret = avcodec_open2(avCodecCtx, avCodec, NULL);
			openAVCodec = true;
		}

		if(ERR_OK == convertYV12ToYUV420P(data, dataBytes, w, h))
		{
			status = encodeJPEG(jpegData, jpegBytes);
		}
		else
		{
			status = ERR_BAD_OPERATE;
		}
	}

	return status;
}

int YV12ToJPEG::encodeJPEG(char*& jpegData, int& jpegBytes)
{
	AVFrame* yuv420PFrame{av_frame_alloc()};

	if(yuv420PFrame)
	{
		yuv420PFrame->format = AV_PIX_FMT_YUV420P;
		yuv420PFrame->width = avCodecCtx->width;
		yuv420PFrame->height = avCodecCtx->height;

		av_image_fill_arrays(
			yuv420PFrame->data, yuv420PFrame->linesize, 
			(const uint8_t*)yuv420PFrameData, AV_PIX_FMT_YUV420P, 
			yuv420PFrame->width, yuv420PFrame->height, 1);

		int ret = avcodec_send_frame(avCodecCtx, yuv420PFrame);
		if(!ret)
		{
			AVPacket pkt;
			av_init_packet(&pkt);
			avcodec_receive_packet(avCodecCtx, &pkt);
			jpegData = new(std::nothrow) char[pkt.size];
			if (jpegData)
			{
				memcpy_s(jpegData, pkt.size, pkt.data, pkt.size);
				jpegBytes = pkt.size;
			}
			av_packet_unref(&pkt);
		}

		av_frame_free(&yuv420PFrame);
	}
	
	return 1;
}

int YV12ToJPEG::convertYV12ToYUV420P(
	const char* data /* = NULL */, const int dataBytes /* = 0 */, const int w /* = 1920 */, const int h /* = 1080 */)
{
	if(!yuv420PFrameData)
	{
		int pixelBufferBytes{av_image_get_buffer_size(AV_PIX_FMT_YUV420P, w, h, 1)};
		yuv420PFrameData = (char*)av_malloc(pixelBufferBytes);
	}
	
	int yStrideBytes = w * h;
	int uvStrideBytes = yStrideBytes / 4;
	int srcUOffset = yStrideBytes;
	int srcVOffset = srcUOffset + uvStrideBytes;
	int dstVOffset = yStrideBytes;
	int dstUOffset = dstVOffset + uvStrideBytes;

	memcpy(yuv420PFrameData, data, yStrideBytes);
	memcpy(yuv420PFrameData + dstVOffset, data + srcVOffset, uvStrideBytes);
	memcpy(yuv420PFrameData + dstUOffset, data + srcUOffset, uvStrideBytes);

	return ERR_OK;
}

NS_END
