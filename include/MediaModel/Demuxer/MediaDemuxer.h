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
#include "MediaModel/MediaModel.h"

NS_BEGIN(model, 1)

class MediaDemuxer : public MediaModel
{
public:
	MediaDemuxer(void);
	virtual ~MediaDemuxer(void);

public:
	virtual int openStream(const std::string streamUrl);
	virtual int closeStream(void);
	virtual const MediaDataSubID getVideoStreamID(void) const
	{
		return MediaDataSubID::MEDIA_DATA_SUB_ID_NONE;
	}
	virtual const MediaDataSubID getAudioStreamID(void) const
	{
		return MediaDataSubID::MEDIA_DATA_SUB_ID_NONE;
	}

protected:
	virtual void pullStreamDataProcess(void) = 0;

private:
	int runPullStreamDataThread(void);
	void pullStreamDataWorkerThread(void);

private:
	bool stopped;
	boost::mutex mtx;
	boost::condition condition;
};//class MediaDemuxer

NS_END

#endif//MEDIA_DEMUXER_H
