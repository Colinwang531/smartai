extern "C"
{
#include "libavformat/avformat.h"
#include "libavutil/pixdesc.h"
}
#include "error.h"
#include "MediaDemuxer/FFmpeg/FFmpegAVMediaDemuxer.h"

NS_BEGIN(demuxer, 1)

FFmpegAVMediaDemuxer::FFmpegAVMediaDemuxer() : MediaDemuxer(), avFormatContext{ NULL }
{}

FFmpegAVMediaDemuxer::~FFmpegAVMediaDemuxer(void)
{
	closeStream();
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

int FFmpegAVMediaDemuxer::openStream(const std::string streamUrl)
{
	int status{ streamUrl.empty() ? ERR_INVALID_PARAM : ERR_OK };

	if (ERR_OK == status)
	{
		if (!avformat_open_input((AVFormatContext**)&avFormatContext, streamUrl.c_str(), NULL, NULL))
		{
			status = (0 <= avformat_find_stream_info((AVFormatContext*)avFormatContext, NULL) ? ERR_OK : ERR_STREAM_TYPE_UNKNOWN);
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
		status = ERR_OK;
	}

	return status;
}

const MediaStreamID FFmpegAVMediaDemuxer::getVideoStreamID() const 
{
	MediaStreamID streamID{ MediaStreamID::MEDIA_STREAM_ID_NONE };

	for (int i = 0; i != ((AVFormatContext*)avFormatContext)->nb_streams; i++)
	{
		if (AVMEDIA_TYPE_VIDEO == ((AVFormatContext*)avFormatContext)->streams[i]->codecpar->codec_type)
		{
			if (AV_CODEC_ID_H264 == ((AVFormatContext*)avFormatContext)->streams[i]->codecpar->codec_id)
			{
				streamID = MediaStreamID::MEDIA_STREAM_ID_H264;
			}
			else if (AV_CODEC_ID_H265 == ((AVFormatContext*)avFormatContext)->streams[i]->codecpar->codec_id)
			{
				streamID = MediaStreamID::MEDIA_STREAM_ID_H265;
			}

			break;
		}
	}

	return streamID;
}

const MediaStreamID FFmpegAVMediaDemuxer::getAudioStreamID() const
{
	MediaStreamID streamID{ MediaStreamID::MEDIA_STREAM_ID_NONE };

	for (int i = 0; i != ((AVFormatContext*)avFormatContext)->nb_streams; i++)
	{
		if (AVMEDIA_TYPE_AUDIO == ((AVFormatContext*)avFormatContext)->streams[i]->codecpar->codec_type)
		{
			if (AV_CODEC_ID_AAC == ((AVFormatContext*)avFormatContext)->streams[i]->codecpar->codec_id)
			{
				streamID = MediaStreamID::MEDIA_STREAM_ID_AAC;
			}
			else if (AV_CODEC_ID_ADPCM_G722 == ((AVFormatContext*)avFormatContext)->streams[i]->codecpar->codec_id)
			{
				streamID = MediaStreamID::MEDIA_STREAM_ID_G722;
			}

			break;
		}
	}

	return streamID;
}

void FFmpegAVMediaDemuxer::pullStreamDataProcess(void)
{
}

NS_END
