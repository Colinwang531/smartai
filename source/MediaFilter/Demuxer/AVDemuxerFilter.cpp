#include "boost/make_shared.hpp"
#include "error.h"
#include "MediaDemuxer/FFmpeg/FFmpegFileMediaDemuxer.h"
#include "MediaPin/OutputMediaPin.h"
#include "MediaFilter/Demuxer/AVDemuxerFilter.h"

NS_BEGIN(filter, 1)

AVDemuxerFilter::AVDemuxerFilter() : SourceMediaFilter()
{}

AVDemuxerFilter::~AVDemuxerFilter()
{}

const long long AVDemuxerFilter::getTotalSeconds() const
{
	long long fileTotalSeconds{ 0 };
	boost::shared_ptr<NS(demuxer, 1)::FFmpegFileMediaDemuxer> ffmpegDemuxerPtr{
		boost::dynamic_pointer_cast<NS(demuxer, 1)::FFmpegFileMediaDemuxer>(mediaDemuxerPtr) };

	if (ffmpegDemuxerPtr)
	{
		fileTotalSeconds = ffmpegDemuxerPtr->getTotalSeconds();
	}

	return fileTotalSeconds;
}

const long long AVDemuxerFilter::getBitrate() const
{
	long long fileBitrate{ 0 };
	boost::shared_ptr<NS(demuxer, 1)::FFmpegFileMediaDemuxer> ffmpegDemuxerPtr{
		boost::dynamic_pointer_cast<NS(demuxer, 1)::FFmpegFileMediaDemuxer>(mediaDemuxerPtr) };

	if (ffmpegDemuxerPtr)
	{
		fileBitrate = ffmpegDemuxerPtr->getBitrate();
	}

	return fileBitrate;
}

const MediaStreamType AVDemuxerFilter::getVideoStreamType(void) const
{
	return MediaStreamType();
}

const MediaStreamType AVDemuxerFilter::getAudioStreamType(void) const
{
	return MediaStreamType();
}

int AVDemuxerFilter::inputMediaData(MediaDataPtr mediaData)
{
	int status{ mediaDemuxerPtr ? ERR_EXISTED : ERR_OK };

	if (ERR_OK == status)
	{
		// if current media data is the first one, create demuxer and output pins instances once.
		status = createNewMediaDemuxer(mediaData);

		if (ERR_OK == status)
		{
			status = createNewOutputPin();
		}
	}

	return status;
}

int AVDemuxerFilter::createNewMediaDemuxer(MediaDataPtr mediaData)
{
	int status{ ERR_BAD_ALLOC };
	NS(media, 1)::MediaDataType mediaDataType{ mediaData->getMediaDataType() };

	if (NS(media, 1)::MediaDataType::MEDIA_DATA_TYPE_FILE_PATH == mediaDataType)
	{
		MediaDemuxerPtr demuxerPtr{ boost::make_shared<NS(demuxer, 1)::FFmpegFileMediaDemuxer>() };
		if (demuxerPtr)
		{
			mediaDemuxerPtr.swap(demuxerPtr);
			status = mediaDemuxerPtr->inputMediaData(mediaData);
		}
	}
	else
	{
		status = ERR_BAD_OPERATE;
	}

	return status;
}

int AVDemuxerFilter::createNewOutputPin()
{
	int status{ ERR_BAD_ALLOC };
	MediaPinPtr videoOutputPinPtr{ boost::make_shared<NS(pin, 1)::OutputMediaPin>() };
	MediaPinPtr audioOutputPinPtr{ boost::make_shared<NS(pin, 1)::OutputMediaPin>() };

	if (videoOutputPinPtr && audioOutputPinPtr)
	{
		mediaPinGroup.insert(NS(pin, 1)::VideoStreamOutputPinID, videoOutputPinPtr);
		mediaPinGroup.insert(NS(pin, 1)::AudioStreamOutputPinID, audioOutputPinPtr);
		status = ERR_OK;
	}

	return status;
}

NS_END
