#include "boost/make_shared.hpp"
#include "error.h"
#include "MediaDemuxer/FFmpeg/FFmpegAVMediaDemuxer.h"
using FFmpegAVMediaDemuxer = NS(demuxer, 1)::FFmpegAVMediaDemuxer;
#include "MediaPin/OutputMediaPin.h"
using OutputMediaPin = NS(pin, 1)::OutputMediaPin;
#include "MediaFilter/Demuxer/AVDemuxerFilter.h"

NS_BEGIN(filter, 1)

AVDemuxerFilter::AVDemuxerFilter() : SourceMediaFilter()
{}

AVDemuxerFilter::~AVDemuxerFilter()
{}

const long long AVDemuxerFilter::getTotalSeconds() const
{
	long long fileTotalSeconds{ 0 };
	boost::shared_ptr<FFmpegAVMediaDemuxer> ffmpegAVMediaDemuxerPtr{
		boost::dynamic_pointer_cast<FFmpegAVMediaDemuxer>(mediaDemuxerPtr) };

	if (ffmpegAVMediaDemuxerPtr)
	{
		fileTotalSeconds = ffmpegAVMediaDemuxerPtr->getTotalSeconds();
	}

	return fileTotalSeconds;
}

const long long AVDemuxerFilter::getBitrate() const
{
	long long fileBitrate{ 0 };
	boost::shared_ptr<FFmpegAVMediaDemuxer> ffmpegAVMediaDemuxerPtr{
		boost::dynamic_pointer_cast<FFmpegAVMediaDemuxer>(mediaDemuxerPtr) };

	if (ffmpegAVMediaDemuxerPtr)
	{
		fileBitrate = ffmpegAVMediaDemuxerPtr->getBitrate();
	}

	return fileBitrate;
}

const MediaStreamID AVDemuxerFilter::getVideoStreamID(void) const
{
	MediaStreamID streamID{ MediaStreamID::MEDIA_STREAM_ID_NONE };
	boost::shared_ptr<FFmpegAVMediaDemuxer> ffmpegAVMediaDemuxerPtr{
		boost::dynamic_pointer_cast<FFmpegAVMediaDemuxer>(mediaDemuxerPtr) };

	if (ffmpegAVMediaDemuxerPtr)
	{
		streamID = ffmpegAVMediaDemuxerPtr->getVideoStreamID();
	}

	return streamID;
}

const MediaStreamID AVDemuxerFilter::getAudioStreamID(void) const
{
	MediaStreamID streamID{ MediaStreamID::MEDIA_STREAM_ID_NONE };
	boost::shared_ptr<FFmpegAVMediaDemuxer> ffmpegAVMediaDemuxerPtr{
		boost::dynamic_pointer_cast<FFmpegAVMediaDemuxer>(mediaDemuxerPtr) };

	if (ffmpegAVMediaDemuxerPtr)
	{
		streamID = ffmpegAVMediaDemuxerPtr->getAudioStreamID();
	}

	return streamID;
}

int AVDemuxerFilter::createNewMediaDemuxer(const std::string streamUrl)
{
	int status{ ERR_BAD_ALLOC };
	
	MediaDemuxerPtr ffmpegAVMediaDemuxerPtr{ boost::make_shared<FFmpegAVMediaDemuxer>() };
	if (ffmpegAVMediaDemuxerPtr)
	{
		mediaDemuxerPtr.swap(ffmpegAVMediaDemuxerPtr);
		status = mediaDemuxerPtr->openStream(streamUrl);
	}

	return ERR_OK == status ? createNewOutputPin() : status;
}

int AVDemuxerFilter::createNewOutputPin()
{
	int status{ ERR_BAD_ALLOC };
	MediaPinPtr videoOutputPinPtr{ boost::make_shared<OutputMediaPin>() };
	MediaPinPtr audioOutputPinPtr{ boost::make_shared<OutputMediaPin>() };

	if (videoOutputPinPtr && audioOutputPinPtr)
	{
		mediaPinGroup.insert(NS(pin, 1)::VideoStreamOutputPinID, videoOutputPinPtr);
		mediaPinGroup.insert(NS(pin, 1)::AudioStreamOutputPinID, audioOutputPinPtr);
		status = ERR_OK;
	}

	return status;
}

NS_END
