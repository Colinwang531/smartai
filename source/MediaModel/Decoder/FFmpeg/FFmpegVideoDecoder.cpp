#include "boost/make_shared.hpp"
extern "C"
{
#include "libavcodec/avcodec.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
}
#include "error.h"
#include "MediaModel/Decoder/FFmpeg/FFmpegVideoDecoder.h"

static enum AVPixelFormat hw_pix_fmt;

static enum AVPixelFormat get_hw_format(AVCodecContext* ctx,
	const enum AVPixelFormat* pix_fmts)
{
	const enum AVPixelFormat* p;

	for (p = pix_fmts; *p != -1; p++) {
		if (*p == hw_pix_fmt)
			return *p;
	}

	return AV_PIX_FMT_NONE;
}

NS_BEGIN(model, 1)

FFmpegVideoDecoder::FFmpegVideoDecoder()
	: MediaDecoder(), avcodecContext{ NULL }, avcodecPacket{ NULL }, avcodecInputFrame{ NULL }, avcodecOutputFrame{NULL},
	swsContext{ NULL }, outputFrameData{ NULL }, outputFrameDataBytes{ 0 }
{}

FFmpegVideoDecoder::~FFmpegVideoDecoder(void)
{
	deinitializeFFmpegAVCodec();
	deinitializeFFmpegPacketAndFrame();
}

int FFmpegVideoDecoder::inputMediaData(MediaDataPtr mediaData)
{
	int status{ ERR_OK };

	if (!avcodecContext)
	{
		status = initializeFFmpegAVCodec(mediaData);
		if (ERR_OK == status)
		{
			status = initializeFFmpegPacketAndFrame(mediaData);
		}
	}

	return ERR_OK == status ? decodeMediaData(mediaData) : status;
}

int FFmpegVideoDecoder::initializeFFmpegAVCodec(MediaDataPtr mediaData)
{
	int status{ ERR_NOT_SUPPORT };
	AVCodecID avcodecID{ AVCodecID::AV_CODEC_ID_NONE };
	const MediaDataSubID mediaDataSubID{ mediaData->getSubID() };
	const int imageWidth{ mediaData->getWidth() }, imageHeight{ mediaData->getHeight() };

	if (MediaDataSubID::MEDIA_DATA_SUB_ID_H264 == mediaDataSubID)
	{
		avcodecID = AV_CODEC_ID_H264;
	}
	else if (MediaDataSubID::MEDIA_DATA_SUB_ID_H265 == mediaDataSubID)
	{
		avcodecID = AV_CODEC_ID_H265;
	}

	if (AV_CODEC_ID_NONE != avcodecID)
	{
		AVCodec* avcodec{ avcodec_find_decoder(avcodecID) };
		if (avcodec)
		{
			AVHWDeviceType type = av_hwdevice_find_type_by_name("cuda");
			//HW
			for (int i = 0;; i++) 
			{
				const AVCodecHWConfig* config = avcodec_get_hw_config(avcodec, i);
				if (config && config->methods & AV_CODEC_HW_CONFIG_METHOD_HW_DEVICE_CTX && config->device_type == type)
				{
					hw_pix_fmt = config->pix_fmt;
					break;
				}
			}

			avcodecContext = avcodec_alloc_context3(avcodec);
			if (avcodecContext)
			{
 				AVCodecContext* ctx{ reinterpret_cast<AVCodecContext*>(avcodecContext) };
				ctx->time_base.num = 1;
				ctx->time_base.den = 25;
				ctx->codec_type = AVMediaType::AVMEDIA_TYPE_VIDEO;
				ctx->width = imageWidth;
				ctx->height = imageHeight;
				ctx->pix_fmt = AVPixelFormat::AV_PIX_FMT_YUV420P;
				ctx->get_format = get_hw_format;

				//HW
				AVBufferRef* hwBufferRef = NULL;
				status = av_hwdevice_ctx_create(&hwBufferRef, type, NULL, NULL, 0);
				if (0 <= status)
				{
					ctx->hw_device_ctx = av_buffer_ref(hwBufferRef);
				}

				status = 0 > avcodec_open2(ctx, avcodec, NULL) ? ERR_BAD_OPERATE : ERR_OK;
				if (ERR_OK == status)
				{
					swsContext = sws_getContext(
						imageWidth, imageHeight, AV_PIX_FMT_YUV420P, imageWidth, imageHeight, AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);
				}
			}
			else
			{
				status = ERR_BAD_ALLOC;
			}
		}
	}

	return status;
}

void FFmpegVideoDecoder::deinitializeFFmpegAVCodec()
{
	if (avcodecContext)
	{
		avcodec_close(reinterpret_cast<AVCodecContext*>(avcodecContext));
	}

	if (swsContext)
	{
		sws_freeContext(reinterpret_cast<SwsContext*>(swsContext));
	}
}

int FFmpegVideoDecoder::initializeFFmpegPacketAndFrame(MediaDataPtr mediaData)
{
	int status{ ERR_OK };
	const int imageWidth{ mediaData->getWidth() }, imageHeight{ mediaData->getHeight() };

	avcodecInputFrame = av_frame_alloc();
	avcodecOutputFrame = av_frame_alloc();
	if (avcodecInputFrame && avcodecOutputFrame)
	{
		AVFrame* frame{ reinterpret_cast<AVFrame*>(avcodecOutputFrame) };
		outputFrameDataBytes = av_image_get_buffer_size(AV_PIX_FMT_YUV420P, imageWidth, imageHeight, 1);
		outputFrameData = reinterpret_cast<unsigned char*>(av_malloc(outputFrameDataBytes));
		av_image_fill_arrays(
			frame->data, frame->linesize, outputFrameData, AV_PIX_FMT_YUV420P, imageWidth, imageHeight, 1);
	}
	else
	{
		status = ERR_BAD_ALLOC;
	}

	avcodecPacket = av_packet_alloc();
	if (avcodecPacket)
	{
		AVPacket* pkt{ reinterpret_cast<AVPacket*>(avcodecPacket) };
		av_init_packet(pkt);
	}
	else
	{
		status = ERR_BAD_ALLOC;
	}

	return status;
}

void FFmpegVideoDecoder::deinitializeFFmpegPacketAndFrame(void)
{
	if (avcodecInputFrame)
	{
		AVFrame* frame{ reinterpret_cast<AVFrame*>(avcodecInputFrame) };
		av_frame_free(&frame);
	}

	if (avcodecOutputFrame)
	{
		AVFrame* frame{ reinterpret_cast<AVFrame*>(avcodecOutputFrame) };
		av_frame_free(&frame);
	}

	if (avcodecPacket)
	{
		AVPacket* pkt{ reinterpret_cast<AVPacket*>(avcodecPacket) };
		av_packet_free(&pkt);
	}

	if (outputFrameData)
	{
		av_free(outputFrameData);
	}
}

int FFmpegVideoDecoder::decodeMediaData(MediaDataPtr mediaData)
{
	const int imageWidth{ mediaData->getWidth() }, imageHeight{ mediaData->getHeight() };
	AVPacket* pkt{ reinterpret_cast<AVPacket*>(mediaData->getRawData()) };
	int ret{ avcodec_send_packet(reinterpret_cast<AVCodecContext*>(avcodecContext), pkt) };

	if (0 > ret)
	{
		return ERR_BAD_OPERATE;
	}

	while (1)
	{
//		AVFrame* inputAVFrame{ reinterpret_cast<AVFrame*>(avcodecInputFrame) };
		AVFrame* inputAVFrame{ av_frame_alloc() };
		AVFrame* outputAVFrame{ av_frame_alloc() };
// 		av_image_fill_arrays(
// 			outputAVFrame->data, outputAVFrame->linesize, outputFrameData, AV_PIX_FMT_YUV420P, imageWidth, imageHeight, 1);
		ret = avcodec_receive_frame(reinterpret_cast<AVCodecContext*>(avcodecContext), inputAVFrame);

		if (AVERROR(EAGAIN) == ret || AVERROR_EOF == ret)
		{
			av_frame_free(&inputAVFrame);
			av_frame_free(&outputAVFrame);
			break;
		}

//		AVFrame* outputAVFrame{ reinterpret_cast<AVFrame*>(avcodecOutputFrame) };
// 		SwsContext* ctx{ reinterpret_cast<SwsContext*>(swsContext) };
// 		const int height{ 
// 			sws_scale(ctx, (uint8_t const* const*)inputAVFrame->data, inputAVFrame->linesize, 
// 			0, imageHeight, outputAVFrame->data, outputAVFrame->linesize) };

		/* retrieve data from GPU to CPU */
		AVFrame* tempAVFrame{ NULL };
		if (inputAVFrame->format == hw_pix_fmt)
		{
			ret = av_hwframe_transfer_data(outputAVFrame, inputAVFrame, 0);
			tempAVFrame = outputAVFrame;
		}
		else
		{
			tempAVFrame = inputAVFrame;
		}
//		
		ret = av_image_copy_to_buffer(outputFrameData, outputFrameDataBytes,
			(const uint8_t* const*)tempAVFrame->data,
			(const int*)tempAVFrame->linesize, (AVPixelFormat)tempAVFrame->format,
			tempAVFrame->width, tempAVFrame->height, 1);

		if (0 <= ret/*height*/ && postMediaDataCallback)
		{
			boost::shared_ptr<NS(media, 1)::MediaData> mediaDataPtr{ 
				boost::make_shared<NS(media, 1)::MediaData>(
					MediaDataMainID::MEDIA_DATA_MAIN_ID_VIDEO, MediaDataSubID::MEDIA_DATA_SUB_ID_YUV420P) };
			if (mediaDataPtr)
			{
				mediaDataPtr->setData(outputFrameData, outputFrameDataBytes);
				mediaDataPtr->setPixel(imageWidth, imageHeight);
				postMediaDataCallback(mediaDataPtr);
			}
		}

		av_frame_free(&inputAVFrame);
		av_frame_free(&outputAVFrame);
	}

	return ERR_OK;
}

NS_END
