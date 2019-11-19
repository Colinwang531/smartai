#include "boost/bind.hpp"
#include "boost/make_shared.hpp"
#include "error.h"
#include "MediaModel/Demuxer/FFmpeg/FFmpegAVMediaDemuxer.h"
using FFmpegAVMediaDemuxer = NS(model, 1)::FFmpegAVMediaDemuxer;
#include "MediaFilter/Demuxer/AVDemuxerFilter.h"

NS_BEGIN(filter, 1)

AVDemuxerFilter::AVDemuxerFilter() : SourceMediaFilter()
{}

AVDemuxerFilter::~AVDemuxerFilter()
{}

int AVDemuxerFilter::createNewFilter(void)
{
	int status{ ERR_BAD_ALLOC };
	MediaModelPtr ffmpegAVMediaDemuxerPtr{ boost::make_shared<FFmpegAVMediaDemuxer>() };

	if (ffmpegAVMediaDemuxerPtr &&
		ERR_OK == createNewOutputPin(NS(pin, 1)::VideoStreamOutputPinID) &&
		ERR_OK == createNewOutputPin(NS(pin, 1)::AudioStreamOutputPinID))
	{
		mediaModelPtr.swap(ffmpegAVMediaDemuxerPtr);
		status = MediaFilter::createNewFilter();
	}
	
	return status;
}

int AVDemuxerFilter::destroyFilter(void)
{
	return ERR_OK;
}

NS_END
