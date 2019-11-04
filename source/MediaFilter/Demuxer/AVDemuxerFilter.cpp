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
