#include "boost/make_shared.hpp"
#include "error.h"
#include "MediaFilter/Demuxer/AVDemuxerFilter.h"
using AVDemuxerFilter = NS(filter, 1)::AVDemuxerFilter;
#include "MediaFilter/Decoder/AVDecoderFilter.h"
using AVDecoderFilter = NS(filter, 1)::AVDecoderFilter;
#include "MediaFilter/Renderer/AVRendererFilter.h"
using AVRendererFilter = NS(filter, 1)::AVRendererFilter;
#include "MediaGraph/PlaybackMediaGraph.h"

NS_BEGIN(graph, 1)

PlaybackStreamMediaGraph::PlaybackStreamMediaGraph() : MediaGraph()
{}

PlaybackStreamMediaGraph::~PlaybackStreamMediaGraph()
{}

int PlaybackStreamMediaGraph::createNewVideoDecoderFilter()
{
	int status{ ERR_NOT_SUPPORT };
	MediaFilterPtr mediaDemuxerFilterPtr{ queryMediaFilterByID(NS(filter, 1)::AVMediaDemuxerFilterID) };
	MediaFilterPtr mediaDecoderFilterPtr{ boost::make_shared<AVDecoderFilter>() };

	if (mediaDemuxerFilterPtr && mediaDecoderFilterPtr)
	{
		boost::shared_ptr<AVDecoderFilter> avdecoderFilterPtr{
			boost::dynamic_pointer_cast<AVDecoderFilter>(mediaDecoderFilterPtr) };
		boost::shared_ptr<AVDemuxerFilter> avdemuxerFilterPtr{ 
			boost::dynamic_pointer_cast<AVDemuxerFilter>(mediaDemuxerFilterPtr) };
		const MediaStreamID streamID{ avdemuxerFilterPtr->getVideoStreamID() };

		if (MediaStreamID::MEDIA_STREAM_ID_H264 == streamID || MediaStreamID::MEDIA_STREAM_ID_H265 == streamID)
		{
			status = avdecoderFilterPtr->createNewMediaDecoder(streamID);
			if (ERR_OK == status)
			{
				status = addMediaFilter(NS(filter, 1)::AVMediaVideoDecoderFilterID, mediaDecoderFilterPtr);
			}
		}
	}

	return status;
}

int PlaybackStreamMediaGraph::createNewAudioDecoderFilter()
{
	int status{ ERR_NOT_SUPPORT };
	MediaFilterPtr mediaDemuxerFilterPtr{ queryMediaFilterByID(NS(filter, 1)::AVMediaDemuxerFilterID) };
	MediaFilterPtr mediaDecoderFilterPtr{ boost::make_shared<AVDecoderFilter>() };

	if (mediaDemuxerFilterPtr && mediaDecoderFilterPtr)
	{
		boost::shared_ptr<AVDecoderFilter> avdecoderFilterPtr{
			boost::dynamic_pointer_cast<AVDecoderFilter>(mediaDecoderFilterPtr) };
		boost::shared_ptr<AVDemuxerFilter> avdemuxerFilterPtr{
			boost::dynamic_pointer_cast<AVDemuxerFilter>(mediaDemuxerFilterPtr) };
		const MediaStreamID streamID{ avdemuxerFilterPtr->getAudioStreamID() };

		if (MediaStreamID::MEDIA_STREAM_ID_AAC == streamID || MediaStreamID::MEDIA_STREAM_ID_G722 == streamID)
		{
			status = avdecoderFilterPtr->createNewMediaDecoder(streamID);
			if (ERR_OK == status)
			{
				status = addMediaFilter(NS(filter, 1)::AVMediaAudioDecoderFilterID, mediaDecoderFilterPtr);
			}
		}
	}

	return status;
}

int PlaybackStreamMediaGraph::createNewVideoRendererFilter(void* hwnd /* = NULL */)
{
	int status{ hwnd ? ERR_OK : ERR_NOT_SUPPORT };
	MediaFilterPtr mediaDemuxerFilterPtr{ queryMediaFilterByID(NS(filter, 1)::AVMediaDemuxerFilterID) };
	MediaFilterPtr mediaRendererFilterPtr{ boost::make_shared<AVRendererFilter>() };

	if (ERR_OK == status && mediaDemuxerFilterPtr && mediaRendererFilterPtr)
	{
		boost::shared_ptr<AVRendererFilter> avrendererFilterPtr{
			boost::dynamic_pointer_cast<AVRendererFilter>(mediaRendererFilterPtr) };
		boost::shared_ptr<AVDemuxerFilter> avdemuxerFilterPtr{
			boost::dynamic_pointer_cast<AVDemuxerFilter>(mediaDemuxerFilterPtr) };
		const MediaStreamID streamID{ avdemuxerFilterPtr->getVideoStreamID() };

		if (MediaStreamID::MEDIA_STREAM_ID_NONE != streamID)
		{
			status = avrendererFilterPtr->createNewMediaRenderer(hwnd);
			if (ERR_OK == status)
			{
				status = addMediaFilter(NS(filter, 1)::AVMediaVideoRendererFilterID, mediaRendererFilterPtr);
			}
		}
	}

	return status;
}

int PlaybackStreamMediaGraph::createNewAudioRendererFilter(void)
{
	int status{ ERR_NOT_SUPPORT };
	MediaFilterPtr mediaDemuxerFilterPtr{ queryMediaFilterByID(NS(filter, 1)::AVMediaDemuxerFilterID) };
	MediaFilterPtr mediaRendererFilterPtr{ boost::make_shared<AVRendererFilter>() };

	if (mediaDemuxerFilterPtr && mediaRendererFilterPtr)
	{
		boost::shared_ptr<AVRendererFilter> avrendererFilterPtr{
			boost::dynamic_pointer_cast<AVRendererFilter>(mediaRendererFilterPtr) };
		boost::shared_ptr<AVDemuxerFilter> avdemuxerFilterPtr{
			boost::dynamic_pointer_cast<AVDemuxerFilter>(mediaDemuxerFilterPtr) };
		const MediaStreamID streamID{ avdemuxerFilterPtr->getAudioStreamID() };

		if (MediaStreamID::MEDIA_STREAM_ID_NONE != streamID)
		{
			status = avrendererFilterPtr->createNewMediaRenderer();
			if (ERR_OK == status)
			{
				status = addMediaFilter(NS(filter, 1)::AVMediaAudioRendererFilterID, mediaRendererFilterPtr);
			}
		}
	}

	return status;
}

NS_END
