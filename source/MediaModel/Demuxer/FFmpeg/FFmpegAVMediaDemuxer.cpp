#include <windows.h>
#include "boost/make_shared.hpp"
extern "C"
{
#include "libavutil/pixdesc.h"
}
#include "error.h"
#include "MediaModel/Demuxer/FFmpeg/FFmpegAVMediaDemuxer.h"

NS_BEGIN(model, 1)

FFmpegAVMediaDemuxer::FFmpegAVMediaDemuxer()
	: MediaDemuxer(), formatctx{ NULL }, videoStreamIndex{ -1 }, audioStreamIndex{ -1 }
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
		if (!avformat_open_input(&formatctx, streamUrl.c_str(), NULL, NULL))
		{
			status = (0 <= avformat_find_stream_info(formatctx, NULL) ? ERR_OK : ERR_STREAM_TYPE_UNKNOWN);
			if (ERR_OK == status)
			{
				av_init_packet(&packet);
				videoStreamIndex = av_find_best_stream(formatctx, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
				audioStreamIndex = av_find_best_stream(formatctx, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
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

	if (ERR_OK == status && formatctx)
	{
		avformat_close_input(&formatctx);
		av_packet_unref(&packet);
		status = ERR_OK;
	}

	return status;
}

const long long FFmpegAVMediaDemuxer::getTotalSeconds(void) const
{
	long long fileTotalSeconds{ 0 };

	if (formatctx && AV_NOPTS_VALUE != formatctx->duration)
	{
		fileTotalSeconds = formatctx->duration + (formatctx->duration <= INT64_MAX - 5000 ? 5000 : 0);
	}

	return fileTotalSeconds;
}

const long long FFmpegAVMediaDemuxer::getBitrate(void) const
{
	long long fileBitrate{ 0 };

	if (formatctx)
	{
		fileBitrate = formatctx->bit_rate;
	}

	return fileBitrate;
}

const MediaDataSubID FFmpegAVMediaDemuxer::getVideoStreamID() const
{
	MediaDataSubID videoSubID{ MediaDemuxer::getVideoStreamID() };

	for (int i = 0; i != formatctx->nb_streams; i++)
	{
		if (AVMEDIA_TYPE_VIDEO == formatctx->streams[i]->codecpar->codec_type)
		{
			if (AV_CODEC_ID_H264 == formatctx->streams[i]->codecpar->codec_id)
			{
				videoSubID = MediaDataSubID::MEDIA_DATA_SUB_ID_H264;
			}
			else if (AV_CODEC_ID_H265 == formatctx->streams[i]->codecpar->codec_id)
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

	for (int i = 0; i != formatctx->nb_streams; i++)
	{
		if (AVMEDIA_TYPE_AUDIO == formatctx->streams[i]->codecpar->codec_type)
		{
			if (AV_CODEC_ID_AAC == formatctx->streams[i]->codecpar->codec_id)
			{
				audioSubID = MediaDataSubID::MEDIA_DATA_SUB_ID_AAC;
			}
			else if (AV_CODEC_ID_ADPCM_G722 == formatctx->streams[i]->codecpar->codec_id)
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
	boost::winapi::DWORD_ lastknownTickTime{ GetTickCount() };

	if (formatctx && 0 == av_read_frame(formatctx, &packet))
	{
		MediaDataMainID mediaDataMainID{ MediaDataMainID::MEDIA_DATA_MAIN_ID_NONE };
		MediaDataSubID mediaDataSubID{ MediaDataSubID::MEDIA_DATA_SUB_ID_NONE };

		if (packet.stream_index == videoStreamIndex)
		{
			mediaDataMainID = MediaDataMainID::MEDIA_DATA_MAIN_ID_VIDEO;
			mediaDataSubID = videoSubID;
		} 
		else if (packet.stream_index == audioStreamIndex)
		{
			mediaDataMainID = MediaDataMainID::MEDIA_DATA_MAIN_ID_AUDIO;
			mediaDataSubID = audioSubID;
		}

		MediaDataPtr mediaDataPtr{ 
			boost::make_shared<NS(media, 1)::MediaData>(mediaDataMainID, mediaDataSubID) };
		if (mediaDataPtr && postInputMediaDataCallback)
		{
			mediaDataPtr->setRawData(&packet);
			mediaDataPtr->setUserData(formatctx);
			postInputMediaDataCallback(mediaDataPtr);
		}

		av_packet_unref(&packet);

		boost::winapi::DWORD_ nowTickTime{ GetTickCount() };
		char text[1024]{ 0 };
		sprintf_s(text, "Tick cout = %d.\r\n", nowTickTime - lastknownTickTime);
		OutputDebugStringA(text);
		lastknownTickTime = nowTickTime;
	}
}

NS_END
