#include <windows.h>
#include "boost/make_shared.hpp"
extern "C"
{
#include "libavformat/avformat.h"
#include "libavutil/pixdesc.h"
}
#include "error.h"
#include "MediaModel/Demuxer/FFmpeg/FFmpegAVMediaDemuxer.h"

NS_BEGIN(model, 1)

FFmpegAVMediaDemuxer::FFmpegAVMediaDemuxer()
	: MediaDemuxer(), avFormatContext{ NULL }, avPacket{ NULL }, 
	videoMediaIndex{ -1 }, audioMediaIndex{ -1 }, imageWidth{ 0 }, imageHeight{ 0 }
{}

FFmpegAVMediaDemuxer::~FFmpegAVMediaDemuxer(void)
{
	closeStream();
}

int FFmpegAVMediaDemuxer::openStream(const std::string streamUrl)
{
	int status{ streamUrl.empty() ? ERR_INVALID_PARAM : ERR_OK };

	if (ERR_OK == status)
	{
		if (!avformat_open_input((AVFormatContext**)&avFormatContext, streamUrl.c_str(), NULL, NULL))
		{
			status = (0 <= avformat_find_stream_info((AVFormatContext*)avFormatContext, NULL) ? ERR_OK : ERR_STREAM_TYPE_UNKNOWN);
			if (ERR_OK == status)
			{
				getMediaDataParameters();
				avPacket = av_malloc(sizeof(AVPacket));
				status = avPacket ? ERR_OK : ERR_BAD_ALLOC;
			}
		}
		else
		{
			status = ERR_FILE_OPEN_FAILED;
		}
	}

	return ERR_OK == status ? MediaDemuxer::openStream(streamUrl) : status;
}

int FFmpegAVMediaDemuxer::closeStream()
{
	int status{ MediaDemuxer::closeStream() };

	if (avFormatContext)
	{
		avformat_close_input((AVFormatContext**)&avFormatContext);
		av_free(avPacket);
		status = ERR_OK;
	}

	return status;
}

const long long FFmpegAVMediaDemuxer::getTotalSeconds(void) const
{
	long long fileTotalSeconds{ 0 };

	if (avFormatContext && AV_NOPTS_VALUE != ((AVFormatContext*)avFormatContext)->duration)
	{
		fileTotalSeconds = ((AVFormatContext*)avFormatContext)->duration + (((AVFormatContext*)avFormatContext)->duration <= INT64_MAX - 5000 ? 5000 : 0);
	}

	return fileTotalSeconds;
}

const long long FFmpegAVMediaDemuxer::getBitrate(void) const
{
	long long fileBitrate{ 0 };

	if (avFormatContext)
	{
		fileBitrate = ((AVFormatContext*)avFormatContext)->bit_rate;
	}

	return fileBitrate;
}

const MediaDataSubID FFmpegAVMediaDemuxer::getVideoStreamID() const
{
	MediaDataSubID videoSubID{ MediaDemuxer::getVideoStreamID() };

	for (int i = 0; i != ((AVFormatContext*)avFormatContext)->nb_streams; i++)
	{
		if (AVMEDIA_TYPE_VIDEO == ((AVFormatContext*)avFormatContext)->streams[i]->codecpar->codec_type)
		{
			if (AV_CODEC_ID_H264 == ((AVFormatContext*)avFormatContext)->streams[i]->codecpar->codec_id)
			{
				videoSubID = MediaDataSubID::MEDIA_DATA_SUB_ID_H264;
			}
			else if (AV_CODEC_ID_H265 == ((AVFormatContext*)avFormatContext)->streams[i]->codecpar->codec_id)
			{
				videoSubID = MediaDataSubID::MEDIA_DATA_SUB_ID_H265;
			}

			break;
		}
	}

	return videoSubID;
}

const MediaDataSubID FFmpegAVMediaDemuxer::getAudioStreamID() const
{
	MediaDataSubID audioSubID{ MediaDemuxer::getAudioStreamID() };

	for (int i = 0; i != ((AVFormatContext*)avFormatContext)->nb_streams; i++)
	{
		if (AVMEDIA_TYPE_AUDIO == ((AVFormatContext*)avFormatContext)->streams[i]->codecpar->codec_type)
		{
			if (AV_CODEC_ID_AAC == ((AVFormatContext*)avFormatContext)->streams[i]->codecpar->codec_id)
			{
				audioSubID = MediaDataSubID::MEDIA_DATA_SUB_ID_AAC;
			}
			else if (AV_CODEC_ID_ADPCM_G722 == ((AVFormatContext*)avFormatContext)->streams[i]->codecpar->codec_id)
			{
				audioSubID = MediaDataSubID::MEDIA_DATA_SUB_ID_G722;
			}

			break;
		}
	}

	return audioSubID;
}

int FFmpegAVMediaDemuxer::inputMediaData(MediaDataPtr mediaData)
{
	return ERR_NOT_SUPPORT;
}

void FFmpegAVMediaDemuxer::pullStreamDataProcess(void)
{
	MediaDataSubID videoSubID{ getVideoStreamID() }, audioSubID{ getAudioStreamID() };
	AVFormatContext* ctx{ reinterpret_cast<AVFormatContext*>(avFormatContext) };
	AVPacket* pkt{ reinterpret_cast<AVPacket*>(avPacket) };
	boost::winapi::DWORD_ lastknownTickTime{ GetTickCount() };

	while (ctx && avPacket && 0 <= av_read_frame(ctx, pkt))
	{
		MediaDataMainID mediaDataMainID{ MediaDataMainID::MEDIA_DATA_MAIN_ID_NONE };
		MediaDataSubID mediaDataSubID{ MediaDataSubID::MEDIA_DATA_SUB_ID_NONE };

		if (pkt->stream_index == videoMediaIndex)
		{
			mediaDataMainID = MediaDataMainID::MEDIA_DATA_MAIN_ID_VIDEO;
			mediaDataSubID = videoSubID;
		} 
		else if (pkt->stream_index == audioMediaIndex)
		{
			mediaDataMainID = MediaDataMainID::MEDIA_DATA_MAIN_ID_AUDIO;
			mediaDataSubID = audioSubID;
			continue;
		}

		MediaDataPtr mediaDataPtr{ 
			boost::make_shared<NS(media, 1)::MediaData>(mediaDataMainID, mediaDataSubID) };
		if (mediaDataPtr && postMediaDataCallback)
		{
			mediaDataPtr->setRawData(pkt);
			mediaDataPtr->setPixel(imageWidth, imageHeight);
			postMediaDataCallback(mediaDataPtr);
		}

		av_packet_unref(pkt);

		boost::winapi::DWORD_ nowTickTime{ GetTickCount() };
		char outputStr[1024]{ 0 };
		sprintf_s(outputStr, "Tick cout = %d.\r\n", nowTickTime - lastknownTickTime);
		OutputDebugStringA(outputStr);
		lastknownTickTime = nowTickTime;
	}
}

void FFmpegAVMediaDemuxer::getMediaDataParameters(void)
{
	for (int i = 0; i != ((AVFormatContext*)avFormatContext)->nb_streams; i++)
	{
		AVCodec* avcodec{ avcodec_find_decoder(((AVFormatContext*)avFormatContext)->streams[i]->codecpar->codec_id) };
		if (avcodec)
		{
			AVCodecContext* avcodecContext{ avcodec_alloc_context3(avcodec) };
			if (avcodecContext && 
				0 <= avcodec_parameters_to_context(
					avcodecContext, ((AVFormatContext*)avFormatContext)->streams[i]->codecpar))
			{
				if (AVMEDIA_TYPE_VIDEO == ((AVFormatContext*)avFormatContext)->streams[i]->codecpar->codec_type)
				{
					imageWidth = avcodecContext->width;
					imageHeight = avcodecContext->height;
					videoMediaIndex = i;
				}
				else if (AVMEDIA_TYPE_AUDIO == ((AVFormatContext*)avFormatContext)->streams[i]->codecpar->codec_type)
				{
					audioMediaIndex = i;
				}
			}

			avcodec_close(avcodecContext);
		}
	}
}

NS_END
