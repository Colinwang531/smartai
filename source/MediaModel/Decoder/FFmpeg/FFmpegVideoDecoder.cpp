#include "boost/make_shared.hpp"
extern "C"
{
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
}
#include "error.h"
#include "MediaModel/Decoder/FFmpeg/FFmpegVideoDecoder.h"

namespace framework
{
	namespace multimedia
	{
		static enum AVPixelFormat pixelformat = AV_PIX_FMT_NONE;

		static AVPixelFormat getHWFormat(
			AVCodecContext* codecctx = NULL, const enum AVPixelFormat* pixelformats = NULL)
		{
			const enum AVPixelFormat* p{ NULL };

			for (p = pixelformats; *p != -1; p++)
			{
				if (*p == pixelformat)
				{
					return *p;
				}
			}

			return AV_PIX_FMT_NONE;
		}

		FFmpegVideoDecoder::FFmpegVideoDecoder()
			: MediaDecoder(), codecctx{ NULL }//, hwdevicectx{ NULL }//, avcodecPacket{ NULL }, avcodecInputFrame{ NULL }, avcodecOutputFrame{NULL},
		//	swsContext{ NULL }, outputFrameData{ NULL }, outputFrameDataBytes{ 0 }
		{}

		FFmpegVideoDecoder::~FFmpegVideoDecoder(void)
		{
			deinitializeFFmpegAVCodec();
			//	deinitializeFFmpegPacketAndFrame();
		}

		int FFmpegVideoDecoder::inputMediaData(MediaDataPtr mediaData)
		{
			int status{ mediaData ? ERR_OK : ERR_INVALID_PARAM };

			if (ERR_OK == status && !codecctx)
			{
				status = initializeVideoDecoder(mediaData);
				// 		if (ERR_OK == status)
				// 		{
				// 			status = initializeFFmpegPacketAndFrame(mediaData);
				// 		}
			}

			return ERR_OK == status ? decodeMediaData(mediaData) : status;
		}

		int FFmpegVideoDecoder::initializeVideoDecoder(MediaDataPtr mediaData)
		{
			// Find hardware device.
			enum AVHWDeviceType deviceType{ av_hwdevice_find_type_by_name("cuda") };
			if (AV_HWDEVICE_TYPE_NONE == deviceType)
			{
				deviceType = av_hwdevice_find_type_by_name("dxva2");
			}

			if (AV_HWDEVICE_TYPE_NONE == deviceType)
			{
				// Software decode.
			}
			else
			{
				// Hardware decode.
				AVBufferRef* devicectx{ av_hwdevice_ctx_alloc(deviceType) };
				if (devicectx && !av_hwdevice_ctx_init(devicectx))
				{
					if (!av_hwdevice_ctx_create(&devicectx, deviceType, NULL, NULL, 0))
					{
						codecctx->hw_device_ctx = av_buffer_ref(devicectx);
					}
				}
			}

			int status{ ERR_NOT_SUPPORT };
// 			AVCodec* codec{ NULL };
// 			AVFormatContext* formatctx{ reinterpret_cast<AVFormatContext*>(mediaData->getUserData()) };
// 
// 			if (MediaDataMainID::MEDIA_DATA_MAIN_ID_VIDEO == mediaData->getMainID() &&
// 				AV_HWDEVICE_TYPE_NONE != devicetype)
// 			{
// 				int streamIndex{ av_find_best_stream(formatctx, AVMEDIA_TYPE_VIDEO, -1, -1, &codec, 0) };
// 				for (int i = 0;; i++)
// 				{
// 					const AVCodecHWConfig* config = avcodec_get_hw_config(codec, i);
// 					if (config && (config->methods & AV_CODEC_HW_CONFIG_METHOD_HW_DEVICE_CTX) && config->device_type == devicetype)
// 					{
// 						pixelformat = config->pix_fmt;
// 						codecctx = avcodec_alloc_context3(codec);
// 
// 						if (codecctx && 0 <= avcodec_parameters_to_context(codecctx, formatctx->streams[streamIndex]->codecpar))
// 						{
// 							codecctx->get_format = getHWFormat;
// 
// 							if (!av_hwdevice_ctx_create(&hwdevicectx, devicetype, NULL, NULL, 0))
// 							{
// 								codecctx->hw_device_ctx = av_buffer_ref(hwdevicectx);
// 							}
// 
// 							if (!avcodec_open2(codecctx, codec, NULL))
// 							{
// 								status = ERR_OK;
// 							}
// 						}
// 
// 						break;
// 					}
// 				}
// 			}

			return status;
		}

		void FFmpegVideoDecoder::deinitializeFFmpegAVCodec()
		{
			if (codecctx)
			{
				avcodec_close(codecctx);
			}

			if (hwdevicectx)
			{
				av_buffer_unref(&hwdevicectx);
			}

			// 	if (swsContext)
			// 	{
			// 		sws_freeContext(reinterpret_cast<SwsContext*>(swsContext));
			// 	}
		}

		int FFmpegVideoDecoder::initializeFFmpegPacketAndFrame(MediaDataPtr mediaData)
		{
			int status{ ERR_OK };
			// 	const int imageWidth{ mediaData->getWidth() }, imageHeight{ mediaData->getHeight() };
			// 
			// 	avcodecInputFrame = av_frame_alloc();
			// 	avcodecOutputFrame = av_frame_alloc();
			// 	if (avcodecInputFrame && avcodecOutputFrame)
			// 	{
			// 		AVFrame* frame{ reinterpret_cast<AVFrame*>(avcodecOutputFrame) };
			// 		outputFrameDataBytes = av_image_get_buffer_size(AV_PIX_FMT_YUV420P, imageWidth, imageHeight, 1);
			// 		outputFrameData = reinterpret_cast<unsigned char*>(av_malloc(outputFrameDataBytes));
			// 		av_image_fill_arrays(
			// 			frame->data, frame->linesize, outputFrameData, AV_PIX_FMT_YUV420P, imageWidth, imageHeight, 1);
			// 	}
			// 	else
			// 	{
			// 		status = ERR_BAD_ALLOC;
			// 	}
			// 
			// 	avcodecPacket = av_packet_alloc();
			// 	if (avcodecPacket)
			// 	{
			// 		AVPacket* pkt{ reinterpret_cast<AVPacket*>(avcodecPacket) };
			// 		av_init_packet(pkt);
			// 	}
			// 	else
			// 	{
			// 		status = ERR_BAD_ALLOC;
			// 	}

			return status;
		}

		void FFmpegVideoDecoder::deinitializeFFmpegPacketAndFrame(void)
		{
			// 	if (avcodecInputFrame)
			// 	{
			// 		AVFrame* frame{ reinterpret_cast<AVFrame*>(avcodecInputFrame) };
			// 		av_frame_free(&frame);
			// 	}
			// 
			// 	if (avcodecOutputFrame)
			// 	{
			// 		AVFrame* frame{ reinterpret_cast<AVFrame*>(avcodecOutputFrame) };
			// 		av_frame_free(&frame);
			// 	}
			// 
			// 	if (avcodecPacket)
			// 	{
			// 		AVPacket* pkt{ reinterpret_cast<AVPacket*>(avcodecPacket) };
			// 		av_packet_free(&pkt);
			// 	}
			// 
			// 	if (outputFrameData)
			// 	{
			// 		av_free(outputFrameData);
			// 	}
		}

		int FFmpegVideoDecoder::decodeMediaData(MediaDataPtr mediaData)
		{
// 			AVPacket* pkt{ reinterpret_cast<AVPacket*>(mediaData->getRawData()) };
// 			int ret{ avcodec_send_packet(codecctx, pkt) };
// 
// 			if (0 > ret)
// 			{
// 				return ERR_BAD_OPERATE;
// 			}
// 
// 			while (1)
// 			{
// 				AVFrame* inputAVFrame{ av_frame_alloc() };
// 				AVFrame* outputAVFrame{ av_frame_alloc() };
// 
// 				if (!inputAVFrame || !outputAVFrame)
// 				{
// 					av_frame_free(&inputAVFrame);
// 					av_frame_free(&outputAVFrame);
// 					break;
// 				}
// 
// 				ret = avcodec_receive_frame(codecctx, inputAVFrame);
// 				if (AVERROR(EAGAIN) == ret || AVERROR_EOF == ret || 0 > ret)
// 				{
// 					av_frame_free(&inputAVFrame);
// 					av_frame_free(&outputAVFrame);
// 					break;
// 				}
// 
// 				/* retrieve data from GPU to CPU */
// 				AVFrame* tempAVFrame{ NULL };
// 				if (inputAVFrame->format == pixelformat)
// 				{
// 					if (0 == av_hwframe_transfer_data(outputAVFrame, inputAVFrame, 0))
// 					{
// 						tempAVFrame = outputAVFrame;
// 					}
// 				}
// 				else
// 				{
// 					tempAVFrame = inputAVFrame;
// 				}
// 
// 				const int imageBufferBytes{
// 					av_image_get_buffer_size((AVPixelFormat)tempAVFrame->format, tempAVFrame->width, tempAVFrame->height, 1) };
// 				uint8_t* imageBuffer{ (uint8_t*)av_malloc(imageBufferBytes) };
// 				if (0 < imageBufferBytes && imageBuffer &&
// 					0 <= av_image_copy_to_buffer(
// 						imageBuffer, imageBufferBytes, (const uint8_t* const*)tempAVFrame->data, (const int*)tempAVFrame->linesize,
// 						(AVPixelFormat)tempAVFrame->format, tempAVFrame->width, tempAVFrame->height, 1))
// 				{
// 					if (postInputMediaDataCallback)
// 					{
// 						boost::shared_ptr<NS(media, 1)::MediaData> mediaDataPtr{
// 							boost::make_shared<NS(media, 1)::MediaData>(
// 								MediaDataMainID::MEDIA_DATA_MAIN_ID_VIDEO, MediaDataSubID::MEDIA_DATA_SUB_ID_YUV420P) };
// 						if (mediaDataPtr)
// 						{
// 							mediaDataPtr->setData(imageBuffer, imageBufferBytes);
// 							mediaDataPtr->setPixel(tempAVFrame->width, tempAVFrame->height);
// 							postInputMediaDataCallback(mediaDataPtr);
// 						}
// 					}
// 				}
// 
// 				av_frame_free(&inputAVFrame);
// 				av_frame_free(&outputAVFrame);
// 				av_freep(&imageBuffer);
// 			}

			return ERR_OK;
		}
	}//namespace multimedia
}//namespace framework
