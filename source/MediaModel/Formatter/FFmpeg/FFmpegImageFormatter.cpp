#include "boost/make_shared.hpp"
extern "C"
{
#include "libavutil/imgutils.h"
}
#include "error.h"
#include "MediaData/MediaData.h"
#include "MediaModel/Formatter/FFmpeg/FFmpegImageFormatter.h"

namespace framework
{
	namespace multimedia
	{
		FFmpegImageFormatter::FFmpegImageFormatter(
			const MediaDataSubID mediaDataSubID /* = MediaDataSubID::MEDIA_DATA_SUB_ID_NONE */)
			: MediaFormatter(), oformat{ mediaDataSubID }, ctx{ NULL }, iframe{ NULL }, oframe{ NULL }, obuffer{ NULL }, obufferBytes{ 0 }
		{}

		FFmpegImageFormatter::~FFmpegImageFormatter()
		{
			destroyImageFormatter();
		}

		int FFmpegImageFormatter::inputMediaData(MediaDataPtr mediaData)
		{
			int status{ mediaData ? ERR_OK : ERR_INVALID_PARAM };

			if (ERR_OK == status)
			{
				int width{ 0 }, height{ 0 };
				mediaData->getImageParameter(width, height);

				if (0 < width && 0 < height)
				{
					if (!ctx)
					{
						status = createNewImageFormatter(mediaData);
					}
					
					if (ERR_OK == status && ctx)
					{
						status = scaleImageData(mediaData);
					}
				}
			}

			return status;
		}

		int FFmpegImageFormatter::createNewImageFormatter(MediaDataPtr mediaData)
		{
			iframe = av_frame_alloc();
			oframe = av_frame_alloc();
			int width{ 0 }, height{ 0 };
			mediaData->getImageParameter(width, height);
			const MediaDataSubID subid{ mediaData->getSubID() };
			enum AVPixelFormat opixelformat { AV_PIX_FMT_NONE }, ipixelformat{ AV_PIX_FMT_NONE };

			if (MediaDataSubID::MEDIA_DATA_SUB_ID_YUV420P == oformat)
			{
				opixelformat = AV_PIX_FMT_YUV420P;
			}
			obufferBytes = av_image_get_buffer_size(opixelformat, width, height, 1);
			if (0 < obufferBytes)
			{
				obuffer = reinterpret_cast<unsigned char*>(av_malloc(obufferBytes));
			}

			if (iframe && oframe && obuffer)
			{
				av_image_fill_arrays(
					oframe->data, oframe->linesize, obuffer, opixelformat, width, height, 1);
				//It should be invoked only once.
				const MediaDataSubID mediaDataSubID{ mediaData->getSubID() };
				if (MediaDataSubID::MEDIA_DATA_SUB_ID_NV12 == mediaDataSubID)
				{
					ipixelformat = AV_PIX_FMT_NV12;
				}

				ctx = sws_getContext(
					width, height, ipixelformat, width, height, opixelformat, SWS_BICUBIC, NULL, NULL, NULL);
			}

			return ctx ? ERR_OK : ERR_BAD_ALLOC;
		}

		void FFmpegImageFormatter::destroyImageFormatter()
		{
			av_frame_free(&iframe);
			av_frame_free(&oframe);
			sws_freeContext(ctx);
		}

		int FFmpegImageFormatter::scaleImageData(MediaDataPtr mediaData)
		{
			int width{ 0 }, height{ 0 };
			mediaData->getImageParameter(width, height);
			enum AVPixelFormat ipixelformat{ AV_PIX_FMT_NONE };
			const MediaDataSubID mediaDataSubID{ mediaData->getSubID() };
			if (MediaDataSubID::MEDIA_DATA_SUB_ID_NV12 == mediaDataSubID)
			{
				ipixelformat = AV_PIX_FMT_NV12;
			}
			av_image_fill_arrays(
				iframe->data, iframe->linesize, mediaData->getData(), ipixelformat, width, height, 1);
			
			int sliceheight{ 
				sws_scale(ctx, (uint8_t const* const*)iframe->data, iframe->linesize, 0, height, oframe->data, oframe->linesize) };
			if (0 < sliceheight)
			{
				MediaDataPtr mediaDataPtr{
					boost::make_shared<MediaData>(mediaData->getMainID(), oformat, mediaData->getPatchID()) };
				if (mediaDataPtr && postInputMediaDataCallback)
				{
					mediaDataPtr->setData(obuffer, obufferBytes);
					mediaDataPtr->setImageParameter(width, height);
					postInputMediaDataCallback(mediaDataPtr);
				}
			}

			return 0 < sliceheight ? ERR_OK : ERR_BAD_OPERATE;
		}
	}//namespace multimedia
}//namespace framework
