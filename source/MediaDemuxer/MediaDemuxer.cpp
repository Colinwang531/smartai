#include "MediaDemuxer/MediaDemuxer.h"

NS_BEGIN(demuxer, 1)

MediaDemuxer::MediaDemuxer()
{}

MediaDemuxer::~MediaDemuxer()
{}

const MediaStreamType MediaDemuxer::getVideoStreamType(void) const
{
	return MediaStreamType::MEDIA_STREAM_TYPE_NONE;
}

const MediaStreamType MediaDemuxer::getAudioStreamType(void) const
{
	return MediaStreamType::MEDIA_STREAM_TYPE_NONE;
}

NS_END
