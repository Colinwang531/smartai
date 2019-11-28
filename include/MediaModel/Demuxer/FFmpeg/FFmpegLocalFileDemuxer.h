// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
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
#include "MediaModel/Demuxer/MediaDemuxer.h"

namespace framework
{
	namespace multimedia
	{
		class FFmpegLocalFileDemuxer : public MediaDemuxer
		{
		public:
			FFmpegLocalFileDemuxer(void);
			virtual ~FFmpegLocalFileDemuxer(void);

		public:
			int openStream(const std::string& streamUrl) override;
			int closeStream(void) override;
			const long long getTotalSeconds(void) const;
			const long long getBitrate(void) const;
			void getVideoSolution(int& width, int& height);
			const MediaDataSubID getVideoStreamID(void) const;
			const MediaDataSubID getAudioStreamID(void) const;

		protected:
			void mediaDemuxerWorkerThread(void) override;

		private:
			boost::mutex mtx;
			boost::condition condition;
			AVFormatContext* formatctx;
		};//class FFmpegLocalFileDemuxer
	}//namespace multimedia
}//namespace framework

#endif//FFMPEG_LOCAL_FILE_DEMUXER_H
