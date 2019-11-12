// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Base class of AV demuxer.
//

#ifndef MEDIA_DEMUXER_H
#define MEDIA_DEMUXER_H

#include "boost/thread/condition.hpp"
#include "MediaFilter/MediaFilter.h"
using MediaFilterRef = boost::weak_ptr<NS(filter, 1)::MediaFilter>;

NS_BEGIN(demuxer, 1)

class MediaDemuxer
{
public:
	MediaDemuxer(void);
	virtual ~MediaDemuxer(void);

public:
	virtual int openStream(const std::string streamUrl);
	virtual int closeStream(void);
	int runPullStreamDataThread(void);
	//Suppose only one type of video/audio stream in media file.
	virtual const MediaStreamID getVideoStreamID(void) const = 0;
	virtual const MediaStreamID getAudioStreamID(void) const = 0;

protected:
	virtual void pullStreamDataProcess(void) = 0;

private:
	void pullStreamDataWorkerThread(void);

private:
	bool stopped;
	boost::mutex mtx;
	boost::condition condition;
};//class MediaDemuxer

NS_END

#endif//MEDIA_DEMUXER_H
