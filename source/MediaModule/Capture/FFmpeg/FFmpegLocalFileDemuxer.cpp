#include "boost/make_shared.hpp"
#include "boost/thread.hpp"
extern "C"
{
#include "libavutil/pixdesc.h"
}
#include "error.h"
#include "MediaModule/Capture/FFmpeg/FFmpegLocalFileDemuxer.h"

namespace framework
{
	namespace multimedia
	{
		FFmpegLocalFileDemuxer::FFmpegLocalFileDemuxer() : MediaCapture(), formatctx{ NULL }, stopped{ false }
		{}

		FFmpegLocalFileDemuxer::~FFmpegLocalFileDemuxer(void)
		{
			stopCapture();
		}

		int FFmpegLocalFileDemuxer::startCapture(const std::string& streamUrl)
		{
			int status{ streamUrl.empty() ? ERR_INVALID_PARAM : ERR_OK };

			if (ERR_OK == status)
			{
				if (!avformat_open_input(&formatctx, streamUrl.c_str(), NULL, NULL))
				{
					status = (0 <= avformat_find_stream_info(formatctx, NULL) ? ERR_OK : ERR_STREAM_TYPE_UNKNOWN);

					if (ERR_OK == status)
					{
						stopped = false;
						boost::thread workerThread{ boost::bind(&FFmpegLocalFileDemuxer::mediaDemuxerWorkerThread, this) };
						workerThread.detach();
					}
				}
				else
				{
					status = ERR_FILE_OPEN_FAILED;
				}
			}

			return status;
		}

		int FFmpegLocalFileDemuxer::stopCapture()
		{
			int status{ ERR_OK };
			stopped = true;
			// Wait for exiting thread normally.
			boost::unique_lock<boost::mutex> lock{ mtx };
			condition.wait(lock);

			if (ERR_OK == status && formatctx)
			{
				avformat_close_input(&formatctx);
			}

			return status;
		}

		const long long FFmpegLocalFileDemuxer::getTotalSeconds(void) const
		{
			long long fileTotalSeconds{ 0 };

			if (formatctx && AV_NOPTS_VALUE != formatctx->duration)
			{
				fileTotalSeconds = formatctx->duration + (formatctx->duration <= INT64_MAX - 5000 ? 5000 : 0);
			}

			return fileTotalSeconds;
		}

		const long long FFmpegLocalFileDemuxer::getBitrate(void) const
		{
			long long fileBitrate{ 0 };

			if (formatctx)
			{
				fileBitrate = formatctx->bit_rate;
			}

			return fileBitrate;
		}

		void FFmpegLocalFileDemuxer::getVideoSolution(int& width, int& height)
		{
			for (int i = 0; i != formatctx->nb_streams; i++)
			{
				if (AVMEDIA_TYPE_VIDEO == formatctx->streams[i]->codecpar->codec_type)
				{
					width = formatctx->streams[i]->codecpar->width;
					height = formatctx->streams[i]->codecpar->height;
					break;
				}
			}
		}

		const MediaDataSubID FFmpegLocalFileDemuxer::getVideoStreamID() const
		{
			MediaDataSubID videoSubID{ MediaDataSubID::MEDIA_DATA_SUB_ID_NONE };

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

		const MediaDataSubID FFmpegLocalFileDemuxer::getAudioStreamID() const
		{
			MediaDataSubID audioSubID{ MediaDataSubID::MEDIA_DATA_SUB_ID_NONE };

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

		void FFmpegLocalFileDemuxer::mediaDemuxerWorkerThread(void)
		{
			MediaDataSubID videoStreamID{ getVideoStreamID() }, audioStreamID{ getAudioStreamID() };
			const int videoStreamIndex{ av_find_best_stream(formatctx, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0) },
				audioStreamIndex{ av_find_best_stream(formatctx, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0) };
			int width{ 0 }, height{ 0 };
			getVideoSolution(width, height);
			AVPacket packet;
			av_init_packet(&packet);
			AVCodecParameters* codecparams{ NULL };

			while (formatctx && 0 == av_read_frame(formatctx, &packet))
			{
				MediaDataMainID mediaDataMainID{ MediaDataMainID::MEDIA_DATA_MAIN_ID_NONE };
				MediaDataSubID mediaDataSubID{ MediaDataSubID::MEDIA_DATA_SUB_ID_NONE };

				if (packet.stream_index == videoStreamIndex)
				{
					mediaDataMainID = MediaDataMainID::MEDIA_DATA_MAIN_ID_VIDEO;
					mediaDataSubID = videoStreamID;
					codecparams = formatctx->streams[videoStreamIndex]->codecpar;
				}
				else if (packet.stream_index == audioStreamIndex)
				{
					mediaDataMainID = MediaDataMainID::MEDIA_DATA_MAIN_ID_AUDIO;
					mediaDataSubID = audioStreamID;
					codecparams = formatctx->streams[audioStreamIndex]->codecpar;
				}

				MediaDataPtr mediaDataPtr{
					boost::make_shared<MediaData>(
						mediaDataMainID, 
						mediaDataSubID, 
						AV_PKT_FLAG_KEY == packet.flags ? MediaDataPatchID::MEDIA_DATA_PATCH_ID_I : MediaDataPatchID::MEDIA_DATA_PATCH_ID_P) };
				if (mediaDataPtr && postInputMediaDataCallback)
				{
					mediaDataPtr->setData(packet.data, packet.size);
					mediaDataPtr->setUserData(codecparams);
					mediaDataPtr->setImageParameter(width, height);
					postInputMediaDataCallback(mediaDataPtr);
				}

				av_packet_unref(&packet);
			}

			condition.notify_one();
		}
	}//namespace multimedia
}//namespace framework
