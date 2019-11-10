#include "boost/make_shared.hpp"
#include "error.h"
#include "MediaFilter/Demuxer/AVDemuxerFilter.h"
using AVDemuxerFilter = NS(filter, 1)::AVDemuxerFilter;
#include "MediaFilter/Decoder/AVDecoderFilter.h"
using AVDecoderFilter = NS(filter, 1)::AVDecoderFilter;
#include "MediaFilter/Renderer/AVRendererFilter.h"
using AVRendererFilter = NS(filter, 1)::AVRendererFilter;
#include "MediaGraph/PlaybackStreamMediaGraph.h"

NS_BEGIN(graph, 1)

PlaybackStreamMediaGraph::PlaybackStreamMediaGraph() : MediaGraph()
{}

PlaybackStreamMediaGraph::~PlaybackStreamMediaGraph()
{}

int PlaybackStreamMediaGraph::createNewVideoDecoderFilter()
{
	int status{ ERR_NOT_SUPPORT };
	MediaFilterPtr demuxerFilterPtr{ queryMediaFilterByID(NS(filter, 1)::AVMediaDemuxerFilterID) };
	MediaFilterPtr videoDecoderFilterPtr{ boost::make_shared<AVDecoderFilter>() };

	if (demuxerFilterPtr && videoDecoderFilterPtr)
	{
		boost::shared_ptr<AVDecoderFilter> avdecoderFilterPtr{
			boost::dynamic_pointer_cast<AVDecoderFilter>(videoDecoderFilterPtr) };
		boost::shared_ptr<AVDemuxerFilter> avdemuxerFilterPtr{ 
			boost::dynamic_pointer_cast<AVDemuxerFilter>(demuxerFilterPtr) };
		const MediaStreamID streamID{ avdemuxerFilterPtr->getVideoStreamID() };

		if (MediaStreamID::MEDIA_STREAM_ID_H264 == streamID || MediaStreamID::MEDIA_STREAM_ID_H265 == streamID)
		{
			status = avdecoderFilterPtr->createNewMediaDecoder(streamID);
			if (ERR_OK == status)
			{
				status = addMediaFilter(NS(filter, 1)::AVMediaH2645DecoderFilterID, videoDecoderFilterPtr);
			}
		}
	}

	return status;
}

int PlaybackStreamMediaGraph::createNewAudioDecoderFilter()
{
	int status{ ERR_NOT_SUPPORT };
	std::string filterID{ NS(filter, 1)::AVMediaAACDecoderFilterID };
	MediaFilterPtr demuxerFilterPtr{ queryMediaFilterByID(NS(filter, 1)::AVMediaDemuxerFilterID) };
	MediaFilterPtr audioDecoderFilterPtr{ boost::make_shared<AVDecoderFilter>() };

	if (audioDecoderFilterPtr && demuxerFilterPtr)
	{
		boost::shared_ptr<AVDecoderFilter> avdecoderFilterPtr{
			boost::dynamic_pointer_cast<AVDecoderFilter>(audioDecoderFilterPtr) };
		boost::shared_ptr<AVDemuxerFilter> avdemuxerFilterPtr{
			boost::dynamic_pointer_cast<AVDemuxerFilter>(demuxerFilterPtr) };
		const MediaStreamID streamID{ avdemuxerFilterPtr->getAudioStreamID() };

		if (MediaStreamID::MEDIA_STREAM_ID_AAC == streamID)
		{
			status = avdecoderFilterPtr->createNewMediaDecoder(streamID);
		}
		if (MediaStreamID::MEDIA_STREAM_ID_G722 == streamID)
		{
			status = avdecoderFilterPtr->createNewMediaDecoder(streamID);
			filterID = NS(filter, 1)::AVMediaG722DecoderFilterID;
		}

		if (ERR_OK == status)
		{
			status = addMediaFilter(filterID, audioDecoderFilterPtr);
		}
	}

	return status;
}

int PlaybackStreamMediaGraph::createNewVideoRendererFilter(void* hwnd /* = NULL */)
{
	int status{ hwnd ? ERR_OK : ERR_NOT_SUPPORT };
	MediaFilterPtr demuxerFilterPtr{ queryMediaFilterByID(NS(filter, 1)::AVMediaDemuxerFilterID) };
	MediaFilterPtr videoRendererFilterPtr{ boost::make_shared<AVRendererFilter>() };

	if (ERR_OK == status && demuxerFilterPtr && videoRendererFilterPtr)
	{
		boost::shared_ptr<AVRendererFilter> avrendererFilterPtr{
			boost::dynamic_pointer_cast<AVRendererFilter>(videoRendererFilterPtr) };
		boost::shared_ptr<AVDemuxerFilter> avdemuxerFilterPtr{
			boost::dynamic_pointer_cast<AVDemuxerFilter>(demuxerFilterPtr) };
		const MediaStreamID streamID{ avdemuxerFilterPtr->getVideoStreamID() };

		if (MediaStreamID::MEDIA_STREAM_ID_NONE != streamID)
		{
			status = avrendererFilterPtr->createNewMediaRenderer(hwnd);
			if (ERR_OK == status)
			{
				status = addMediaFilter(NS(filter, 1)::AVMediaVideoRendererFilterID, avrendererFilterPtr);
			}
		}
	}

	return status;
}

int PlaybackStreamMediaGraph::createNewAudioRendererFilter(void)
{
	int status{ ERR_NOT_SUPPORT };
	std::string filterID{ NS(filter, 1)::AVMediaAACDecoderFilterID };
	MediaFilterPtr demuxerFilterPtr{ queryMediaFilterByID(NS(filter, 1)::AVMediaDemuxerFilterID) };
	MediaFilterPtr audioRendererFilterPtr{ boost::make_shared<AVRendererFilter>() };

	if (audioRendererFilterPtr && demuxerFilterPtr)
	{
		boost::shared_ptr<AVRendererFilter> avrendererFilterPtr{
			boost::dynamic_pointer_cast<AVRendererFilter>(audioRendererFilterPtr) };
		boost::shared_ptr<AVDemuxerFilter> avdemuxerFilterPtr{
			boost::dynamic_pointer_cast<AVDemuxerFilter>(demuxerFilterPtr) };
		const MediaStreamID streamID{ avdemuxerFilterPtr->getAudioStreamID() };

		if (MediaStreamID::MEDIA_STREAM_ID_NONE != streamID)
		{
			status = avrendererFilterPtr->createNewMediaRenderer();
			if (ERR_OK == status)
			{
				status = addMediaFilter(NS(filter, 1)::AVMediaAudioRendererFilterID, avrendererFilterPtr);
			}
		}
	}

	return status;
}

NS_END
