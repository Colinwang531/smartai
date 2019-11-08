extern "C"
{
#include "libavformat/avformat.h"
#include "libavutil/pixdesc.h"
}
#include "error.h"
#include "MediaDemuxer/FFmpeg/FFmpegAVMediaDemuxer.h"

NS_BEGIN(demuxer, 1)

FFmpegAVMediaDemuxer::FFmpegAVMediaDemuxer()
	: MediaDemuxer(), avFormatContext{ NULL }, avPacket{ NULL }, videoMediaIndex{ -1 }, audioMediaIndex{ -1 }
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
			if (ERR_OK == status)
			{
				getVideoAndAudioMediaIndex();
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
	AVFormatContext* ctx{ reinterpret_cast<AVFormatContext*>(avFormatContext) };
	AVPacket* pkt{ reinterpret_cast<AVPacket*>(avPacket) };

	while (ctx && avPacket && 0 <= av_read_frame(ctx, pkt))
	{
		if (pkt->stream_index == videoMediaIndex)
		{
			printf("*****	Read VIDEO stream %d bytes.\r\n", pkt->size);
		} 
		else if (pkt->stream_index == audioMediaIndex)
		{
			printf("#####	Read AUDIO stream %d bytes.\r\n", pkt->size);
		}

		av_packet_unref(pkt);
	}
}

void FFmpegAVMediaDemuxer::getVideoAndAudioMediaIndex(void)
{
	for (int i = 0; i != ((AVFormatContext*)avFormatContext)->nb_streams; i++)
	{
		if (AVMEDIA_TYPE_VIDEO == ((AVFormatContext*)avFormatContext)->streams[i]->codecpar->codec_type)
		{
			videoMediaIndex = i;
		}
		else if (AVMEDIA_TYPE_AUDIO == ((AVFormatContext*)avFormatContext)->streams[i]->codecpar->codec_type)
		{
			audioMediaIndex = i;
		}
	}
}

NS_END
