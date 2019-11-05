extern "C"
{
#include "libavformat/avformat.h"
#include "libavutil/pixdesc.h"
}
#include "error.h"
#include "MediaDemuxer/FFmpeg/FFmpegFileMediaDemuxer.h"

NS_BEGIN(demuxer, 1)

FFmpegFileMediaDemuxer::FFmpegFileMediaDemuxer() : MediaDemuxer(), avFormatContext{ NULL }
{}

FFmpegFileMediaDemuxer::~FFmpegFileMediaDemuxer(void)
{
	if (avFormatContext)
	{
		avformat_close_input((AVFormatContext**)&avFormatContext);
	}
}

const long long FFmpegFileMediaDemuxer::getTotalSeconds(void) const
{
	long long fileTotalSeconds{ 0 };

	if (avFormatContext && AV_NOPTS_VALUE != ((AVFormatContext*)avFormatContext)->duration)
	{
		fileTotalSeconds = ((AVFormatContext*)avFormatContext)->duration + (((AVFormatContext*)avFormatContext)->duration <= INT64_MAX - 5000 ? 5000 : 0);
	}

	return fileTotalSeconds;
}

const long long FFmpegFileMediaDemuxer::getBitrate(void) const
{
	long long fileBitrate{ 0 };

	if (avFormatContext)
	{
		fileBitrate = ((AVFormatContext*)avFormatContext)->bit_rate;
	}

	return fileBitrate;
}

int FFmpegFileMediaDemuxer::inputMediaData(MediaDataPtr mediaData)
{
	int status{ ERR_OK };
	boost::weak_ptr<char[]> filePathRef{ mediaData->getMediaDataBuffer() };

	if (!filePathRef.expired())
	{
		if (!avformat_open_input((AVFormatContext**)&avFormatContext, filePathRef.lock().get(), NULL, NULL))
		{
			status = (0 <= avformat_find_stream_info((AVFormatContext*)avFormatContext, NULL) ? ERR_OK : ERR_STREAM_TYPE_UNKNOWN);
		}
		else
		{
			status = ERR_FILE_OPEN_FAILED;
		}
	}

	return status;
}

const MediaStreamType FFmpegFileMediaDemuxer::getVideoStreamType() const 
{
	MediaStreamType mediaStreamType{ MediaStreamType::MEDIA_STREAM_TYPE_NONE };

	for (int i = 0; i != ((AVFormatContext*)avFormatContext)->nb_streams; i++)
	{
		if (AVMEDIA_TYPE_VIDEO == ((AVFormatContext*)avFormatContext)->streams[i]->codecpar->codec_type)
		{
			if (AV_CODEC_ID_H264 == ((AVFormatContext*)avFormatContext)->streams[i]->codecpar->codec_id)
			{
				mediaStreamType = MediaStreamType::MEDIA_STREAM_TYPE_H264;
			}
			else if (AV_CODEC_ID_H265 == ((AVFormatContext*)avFormatContext)->streams[i]->codecpar->codec_id)
			{
				mediaStreamType = MediaStreamType::MEDIA_STREAM_TYPE_H265;
			}

			break;
		}
	}

	return mediaStreamType;
}

const MediaStreamType FFmpegFileMediaDemuxer::getAudioStreamType() const
{
	MediaStreamType mediaStreamType{ MediaStreamType::MEDIA_STREAM_TYPE_NONE };

	for (int i = 0; i != ((AVFormatContext*)avFormatContext)->nb_streams; i++)
	{
		if (AVMEDIA_TYPE_AUDIO == ((AVFormatContext*)avFormatContext)->streams[i]->codecpar->codec_type)
		{
			if (AV_CODEC_ID_AAC == ((AVFormatContext*)avFormatContext)->streams[i]->codecpar->codec_id)
			{
				mediaStreamType = MediaStreamType::MEDIA_STREAM_TYPE_AAC;
			}
			else if (AV_CODEC_ID_ADPCM_G722 == ((AVFormatContext*)avFormatContext)->streams[i]->codecpar->codec_id)
			{
				mediaStreamType = MediaStreamType::MEDIA_STREAM_TYPE_G722;
			}

			break;
		}
	}

	return mediaStreamType;
}

NS_END
