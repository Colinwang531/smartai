#include "boost/make_shared.hpp"
#include "error.h"
#include "MediaFilter/Demuxer/DemuxerFilter.h"
using DemuxerFilter = NS(filter, 1)::DemuxerFilter;
#include "MediaPin/InputMediaPin.h"
using InputMediaPin = NS(pin, 1)::InputMediaPin;
#include "MediaPin/OutputMediaPin.h"
using OutputMediaPin = NS(pin, 1)::OutputMediaPin;
#include "MediaGraph/File/FFmpegFileMediaGraph.h"

NS_BEGIN(graph, 1)

// static int64_t get_bit_rate(AVCodecContext* ctx)
// {
// 	int64_t bit_rate;
// 	int bits_per_sample;
// 
// 	switch (ctx->codec_type) {
// 	case AVMEDIA_TYPE_VIDEO:
// 	case AVMEDIA_TYPE_DATA:
// 	case AVMEDIA_TYPE_SUBTITLE:
// 	case AVMEDIA_TYPE_ATTACHMENT:
// 		bit_rate = ctx->bit_rate;
// 		break;
// 	case AVMEDIA_TYPE_AUDIO:
// 		bits_per_sample = av_get_bits_per_sample(ctx->codec_id);
// 		bit_rate = bits_per_sample ? ctx->sample_rate * (int64_t)ctx->channels * bits_per_sample : ctx->bit_rate;
// 		break;
// 	default:
// 		bit_rate = 0;
// 		break;
// 	}
// 	return bit_rate;
// }

FFmpegFileMediaGraph::FFmpegFileMediaGraph() : FileMediaGraph()
{}

FFmpegFileMediaGraph::~FFmpegFileMediaGraph()
{}

int FFmpegFileMediaGraph::inputMediaData(MediaDataPtr mediaData)
{
	int status{ mediaData ? ERR_OK : ERR_INVALID_PARAM };

	if (ERR_OK == status)
	{
		MediaFilterRef demuxerFilterPtr{ queryMediaFilterByID(NS(filter, 1)::MediaDemuxerFilterID) };
		// Media filter instance does not exist if expired() return true.
		status = demuxerFilterPtr.expired() ?
			createNewDemuxerFilter(mediaData) : demuxerFilterPtr.lock()->inputMediaData(mediaData);
	}

	return status;
}

unsigned long long FFmpegFileMediaGraph::getTotalSeconds()
{
//	int status{ ERR_BAD_OPERATE };

// 	if (avFormatContext && AV_NOPTS_VALUE != ((AVFormatContext*)avFormatContext)->duration)
// 	{
// 		const long long duration{ 
// 			((AVFormatContext*)avFormatContext)->duration + (((AVFormatContext*)avFormatContext)->duration <= INT64_MAX - 5000 ? 5000 : 0) };
// 		second = duration / AV_TIME_BASE;
// //		us = duration % AV_TIME_BASE;
// 		minute = second / 60;
// 		second %= 60;
// 		hour = minute / 60;
// 		minute %= 60;
// 		status = ERR_OK;
// 	}

	return 0;
}

unsigned long long FFmpegFileMediaGraph::getBitrate()
{
// 	int status{ ERR_BAD_OPERATE };
// 
// 	if (avFormatContext)
// 	{
// 		bitrate = ((AVFormatContext*)avFormatContext)->bit_rate;
// 		status = ERR_OK;
// 	}

	return 0;
}

int FFmpegFileMediaGraph::createNewDemuxerFilter(MediaDataPtr mediaData)
{
	int status{ ERR_BAD_ALLOC };
	MediaFilterPtr demuxerFilterPtr{ boost::make_shared<DemuxerFilter>() };

	if (demuxerFilterPtr)
	{
		status = demuxerFilterPtr->inputMediaData(mediaData);
		mediaFilterGroup.insert(NS(filter, 1)::MediaDemuxerFilterID, demuxerFilterPtr);
	}

	return status;
}

void FFmpegFileMediaGraph::buildDecoderFilter()
{
	for (int i = 0; i != ((AVFormatContext*)avFormatContext)->nb_streams; i++)
	{
		AVStream* stream = avFormatContext->streams[i];
		AVCodec* dec = avcodec_find_decoder(stream->codecpar->codec_id);
		AVCodecContext* codec_ctx;
		if (!dec) {
			av_log(NULL, AV_LOG_ERROR, "Failed to find decoder for stream #%u\n", i);
			return AVERROR_DECODER_NOT_FOUND;
		}
		codec_ctx = avcodec_alloc_context3(dec);
		if (!codec_ctx) {
			av_log(NULL, AV_LOG_ERROR, "Failed to allocate the decoder context for stream #%u\n", i);
			return AVERROR(ENOMEM);
		}
		int ret = avcodec_parameters_to_context(codec_ctx, stream->codecpar);
		if (ret < 0) {
			av_log(NULL, AV_LOG_ERROR, "Failed to copy decoder parameters to input decoder context "
				"for stream #%u\n", i);
			return ret;
		}
		/* Reencode video & audio and remux subtitles etc. */
		if (codec_ctx->codec_type == AVMEDIA_TYPE_VIDEO
			|| codec_ctx->codec_type == AVMEDIA_TYPE_AUDIO) {

			const char* codec_type;
			const char* codec_name;
			const char* profile = NULL;
			const char* pix_fmt = NULL;
			codec_type = av_get_media_type_string(codec_ctx->codec_type);
			codec_name = avcodec_get_name(codec_ctx->codec_id);
			profile = avcodec_profile_name(codec_ctx->codec_id, codec_ctx->profile);

			pix_fmt = av_get_pix_fmt_name(codec_ctx->pix_fmt);

			int64_t bitrate = get_bit_rate(codec_ctx);

			av_log(NULL, AV_LOG_INFO, "#Stream %d\n    Type:%s\n    Codec Format:%s\n", i, codec_type, codec_name);
			if (profile)
				av_log(NULL, AV_LOG_INFO, "    Profile:%s\n", profile);

			if (codec_ctx->codec_type == AVMEDIA_TYPE_VIDEO) {
				//resolution ratio
				if (codec_ctx->width)
					av_log(NULL, AV_LOG_INFO, "    Resolution Ratio:%dx%d\n", codec_ctx->width, codec_ctx->height);

				av_log(NULL, AV_LOG_INFO, "    Pix_fmt:%s\n", codec_ctx->pix_fmt == AV_PIX_FMT_NONE ? "none" : pix_fmt);
				//bitrate
				if (bitrate != 0) {
					av_log(NULL, AV_LOG_INFO, "    Bitrate: %d kb/s\n", bitrate / 1000);
				}
				else if (codec_ctx->rc_max_rate > 0) {
					av_log(NULL, AV_LOG_INFO, "    Max Bitrate: %d kb/s\n", (int64_t)codec_ctx->rc_max_rate / 1000);
				}
				float temp = (float)stream->avg_frame_rate.num / (float)stream->avg_frame_rate.den;
				av_log(NULL, AV_LOG_INFO, "    Frame rate:%.2ffps(%d/%d)\n", temp, stream->avg_frame_rate.num, stream->avg_frame_rate.den);

				if (codec_ctx->color_range != AVCOL_RANGE_UNSPECIFIED)
					av_log(NULL, AV_LOG_INFO, "    Color Range:%s\n", av_color_range_name(codec_ctx->color_range));
				//color space
				if (codec_ctx->colorspace != AVCOL_SPC_UNSPECIFIED)
					av_log(NULL, AV_LOG_INFO, "Color Soace:%s\n ", av_get_colorspace_name(codec_ctx->colorspace));
				/*
				 *DAR - display aspect ratio就是视频播放时，我们看到的图像宽高的比例
				 *SAR - storage aspect ratio就是对图像采集时，横向采集与纵向采集构成的点阵，横向点数与纵向点数的比值。
				 */
				AVRational display_aspect_ratio;
				if (codec_ctx->sample_aspect_ratio.num) {
					av_reduce(&display_aspect_ratio.num, &display_aspect_ratio.den,
						codec_ctx->width * (int64_t)codec_ctx->sample_aspect_ratio.num,
						codec_ctx->height * (int64_t)codec_ctx->sample_aspect_ratio.den,
						1024 * 1024);
					av_log(NULL, AV_LOG_INFO, "    SAR: %d:%d\n    DAR: %d:%d\n", codec_ctx->sample_aspect_ratio.num, codec_ctx->sample_aspect_ratio.den,
						display_aspect_ratio.num, display_aspect_ratio.den);
				}

			}
			else if (codec_ctx->codec_type == AVMEDIA_TYPE_AUDIO) {
				if (bitrate != 0) {
					av_log(NULL, AV_LOG_INFO, "    Bitrate: %d kb/s\n", bitrate / 1000);
				}
				else if (codec_ctx->rc_max_rate > 0) {
					av_log(NULL, AV_LOG_INFO, "    Max Bitrate: %d kb/s\n", (int64_t)codec_ctx->rc_max_rate / 1000);
				}

				float temp = (float)codec_ctx->sample_aspect_ratio.num / (float)codec_ctx->sample_aspect_ratio.den;
				av_log(NULL, AV_LOG_INFO, "    Sample ratio: %d Hz,\n", codec_ctx->sample_rate / 1000);

				av_log(NULL, AV_LOG_INFO, "    Channel(s): %d channels\n    Channel Layout %d layouts\n", codec_ctx->channels, codec_ctx->channel_layout);

				if (codec_ctx->sample_fmt != AV_SAMPLE_FMT_NONE) {
					av_log(NULL, AV_LOG_INFO, "    Sample Format :%s\n", av_get_sample_fmt_name(codec_ctx->sample_fmt));
				}
			}

			avcodec_close(codec_ctx);
		}
	}
}

NS_END
