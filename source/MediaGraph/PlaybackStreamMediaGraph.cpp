#include "boost/make_shared.hpp"
#include "error.h"
#include "MediaFilter/Demuxer/AVDemuxerFilter.h"
using AVDemuxerFilter = NS(filter, 1)::AVDemuxerFilter;
#include "MediaFilter/Decoder/AVDecoderFilter.h"
using AVDecoderFilter = NS(filter, 1)::AVDecoderFilter;
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

// int FileMediaGraph::startPlay()
// {
// 	return ERR_OK;
// }
// 
// int FileMediaGraph::stopPlay()
// {
// 	return ERR_OK;
// }
// 
// int FileMediaGraph::pausePlay()
// {
// 	return ERR_OK;
// }
// 
// int FileMediaGraph::fastPlay(const short speed /* = 1 */)
// {
// 	return checkSpeedValue(speed);
// }
// 
// int FileMediaGraph::slowPlay(const short speed /* = -1 */)
// {
// 	return checkSpeedValue(speed);
// }
// 
// int FileMediaGraph::checkSpeedValue(const short speed /* = 1 */)
// {
// 	return 1 == speed || 2 == speed || 4 == speed || 8 == speed || 16 == speed ||
// 		-1 == speed || -2 == speed || -4 == speed || -8 == speed || -16 == speed ?
// 		ERR_OK : ERR_INVALID_PARAM;
// }

NS_END
