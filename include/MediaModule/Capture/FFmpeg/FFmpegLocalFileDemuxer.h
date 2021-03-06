// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : ������
// E-mail : wangkw531@icloud.com
//
// Local file demuxer using FFMPEG library.
//

#ifndef FFMPEG_LOCAL_FILE_DEMUXER_H
#define FFMPEG_LOCAL_FILE_DEMUXER_H

#include "boost/thread/condition.hpp"
extern "C"
{
#include "libavformat/avformat.h"
}
#include "MediaData/MediaData.h"
#include "MediaModule/Capture/MediaCapture.h"

namespace framework
{
	namespace multimedia
	{
		class FFmpegLocalFileDemuxer : public MediaCapture
		{
		public:
			FFmpegLocalFileDemuxer(void);
			virtual ~FFmpegLocalFileDemuxer(void);

		public:
			int startCapture(const std::string& streamUrl) override;
			int stopCapture(void) override;
			const long long getTotalSeconds(void) const;
			const long long getBitrate(void) const;
			void getVideoSolution(int& width, int& height);
			const MediaDataSubID getVideoStreamID(void) const;
			const MediaDataSubID getAudioStreamID(void) const;

		protected:
			void mediaDemuxerWorkerThread(void);

		private:
			boost::mutex mtx;
			boost::condition condition;
			AVFormatContext* formatctx;
			bool stopped;
		};//class FFmpegLocalFileDemuxer
	}//namespace multimedia
}//namespace framework

#endif//FFMPEG_LOCAL_FILE_DEMUXER_H
