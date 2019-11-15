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
	MediaModelPtr ffmpegAVMediaDemuxerPtr{ boost::make_shared<FFmpegAVMediaDemuxer>() };
	if (ffmpegAVMediaDemuxerPtr)
	{
		createNewOutputPin(NS(pin, 1)::VideoStreamOutputPinID);
		createNewOutputPin(NS(pin, 1)::AudioStreamOutputPinID);
		mediaModelPtr.swap(ffmpegAVMediaDemuxerPtr);
	}
	
	return MediaFilter::createNewFilter();
}

int AVDemuxerFilter::destroyFilter(void)
{
	return ERR_OK;
}

NS_END
