#include "boost/make_shared.hpp"
#include "error.h"
#include "MediaFilter/Demuxer/AVDemuxerFilter.h"
using AVDemuxerFilter = NS(filter, 1)::AVDemuxerFilter;
#include "MediaFilter/Decoder/AVDecoderFilter.h"
using AVDecoderFilter = NS(filter, 1)::AVDecoderFilter;
#include "MediaPin/InputMediaPin.h"
using InputMediaPin = NS(pin, 1)::InputMediaPin;
#include "MediaPin/OutputMediaPin.h"
using OutputMediaPin = NS(pin, 1)::OutputMediaPin;
#include "MediaGraph/File/FFmpegFileMediaGraph.h"

NS_BEGIN(graph, 1)

FFmpegFileMediaGraph::FFmpegFileMediaGraph() : FileMediaGraph()
{}

FFmpegFileMediaGraph::~FFmpegFileMediaGraph()
{}

int FFmpegFileMediaGraph::inputMediaData(MediaDataPtr mediaData)
{
	int status{ mediaData ? ERR_OK : ERR_INVALID_PARAM };

	if (ERR_OK == status)
	{
		MediaFilterRef demuxerFilterRef{ queryMediaFilterByID(NS(filter, 1)::AVMediaDemuxerFilterID) };
		// Media filter instance does not exist if expired() return true.
		status = demuxerFilterRef.expired() ? createNewDemuxerFilter(mediaData) : ERR_EXISTED;

		if (ERR_OK == status)
		{
			boost::shared_ptr<AVDemuxerFilter> demuxerFilterPtr{
				boost::dynamic_pointer_cast<AVDemuxerFilter>(demuxerFilterRef.lock()) };
			if (demuxerFilterPtr)
			{
				// Create decoder filters for decoding video and audio according to media stream type.
				createNewVideoDecoderFilter(demuxerFilterPtr->getVideoStreamType());
				createNewAudioDecoderFilter(demuxerFilterPtr->getAudioStreamType());
			}
		}
	}

	return status;
}

const long long FFmpegFileMediaGraph::getTotalSeconds()
{
	unsigned long long fileTotalSeconds{ 0 };

	MediaFilterRef demuxerFilterRef{ queryMediaFilterByID(NS(filter, 1)::AVMediaDemuxerFilterID) };
	if (!demuxerFilterRef.expired())
	{
		boost::shared_ptr<AVDemuxerFilter> demuxerFilterPtr{ 
			boost::dynamic_pointer_cast<AVDemuxerFilter>(demuxerFilterRef.lock()) };
		if (demuxerFilterPtr)
		{
			fileTotalSeconds = demuxerFilterPtr->getTotalSeconds();
		}
	}

	return fileTotalSeconds;
}

const long long FFmpegFileMediaGraph::getBitrate()
{
	unsigned long long fileBitrate{ 0 };

	MediaFilterRef demuxerFilterRef{ queryMediaFilterByID(NS(filter, 1)::AVMediaDemuxerFilterID) };
	if (!demuxerFilterRef.expired())
	{
		boost::shared_ptr<AVDemuxerFilter> demuxerFilterPtr{
			boost::dynamic_pointer_cast<AVDemuxerFilter>(demuxerFilterRef.lock()) };
		if (demuxerFilterPtr)
		{
			fileBitrate = demuxerFilterPtr->getBitrate();
		}
	}

	return fileBitrate;
}

int FFmpegFileMediaGraph::createNewDemuxerFilter(MediaDataPtr mediaData)
{
	int status{ ERR_BAD_ALLOC };
	MediaFilterPtr demuxerFilterPtr{ boost::make_shared<AVDemuxerFilter>() };

	if (demuxerFilterPtr)
	{
		status = demuxerFilterPtr->inputMediaData(mediaData);
		mediaFilterGroup.insert(NS(filter, 1)::AVMediaDemuxerFilterID, demuxerFilterPtr);
	}

	return status;
}

int FFmpegFileMediaGraph::createNewVideoDecoderFilter(
	const MediaStreamType videoType /* = MediaStreamType::MEDIA_STREAM_TYPE_NONE */)
{
	int status{ ERR_NOT_SUPPORT };
	MediaFilterPtr decoderFilterPtr{ boost::make_shared<AVDecoderFilter>() };

	if (decoderFilterPtr)
	{
		boost::shared_ptr<AVDecoderFilter> decoderPtr{
			boost::dynamic_pointer_cast<AVDecoderFilter>(decoderFilterPtr) };

		if (MediaStreamType::MEDIA_STREAM_TYPE_H264 == videoType || MediaStreamType::MEDIA_STREAM_TYPE_H265 == videoType)
		{
			status = decoderPtr->createNewMediaDecoder(MediaDecodeType::MEDIA_DECODE_TYPE_H2645);
			if (ERR_OK == status)
			{
				mediaFilterGroup.insert(NS(filter, 1)::AVMediaH2645DecoderFilterID, decoderFilterPtr);
			}
		}
	}

	return status;
}

int FFmpegFileMediaGraph::createNewAudioDecoderFilter(
	const MediaStreamType audioType /* = MediaStreamType::MEDIA_STREAM_TYPE_NONE */)
{
	int status{ ERR_NOT_SUPPORT };
	std::string filterID{ NS(filter, 1)::AVMediaAACDecoderFilterID };
	MediaFilterPtr decoderFilterPtr{ boost::make_shared<AVDecoderFilter>() };

	if (decoderFilterPtr)
	{
		boost::shared_ptr<AVDecoderFilter> decoderPtr{
			boost::dynamic_pointer_cast<AVDecoderFilter>(decoderFilterPtr) };

		if (MediaStreamType::MEDIA_STREAM_TYPE_AAC == audioType)
		{
			status = decoderPtr->createNewMediaDecoder(MediaDecodeType::MEDIA_DECODE_TYPE_AAC);
		}
		if (MediaStreamType::MEDIA_STREAM_TYPE_G722 == audioType)
		{
			status = decoderPtr->createNewMediaDecoder(MediaDecodeType::MEDIA_DECODE_TYPE_G722);
			filterID = NS(filter, 1)::AVMediaG722DecoderFilterID;
		}

		if (ERR_OK == status)
		{
			mediaFilterGroup.insert(filterID, decoderFilterPtr);
		}
	}

	return status;
}

NS_END
