#include "boost/make_shared.hpp"
#include "error.h"
#include "MediaGraph/FFmpeg/FFmpegPlaybackMediaGraph.h"
using MediaGraphPtr = boost::shared_ptr<framework::multimedia::MediaGraph>;
using FFmpegPlaybackMediaGraph = framework::multimedia::FFmpegPlaybackMediaGraph;
// #include "MediaGraph/LivestreamMediaGraph.h"
// using LivestreamMediaGraph = NS(graph, 1)::LivestreamMediaGraph;
// #include "MediaModel/Accessor/MediaAccessor.h"
// using MediaAccessor = NS(model, 1)::MediaAccessor;
#include "MediaModel/Demuxer/MediaDemuxer.h"
// using MediaDemuxer = NS(model, 1)::MediaDemuxer;
// #include "MediaModel/Renderer/D3D/VideoD3DRenderer.h"
// using VideoD3DRenderer = NS(model, 1)::VideoD3DRenderer;
#include "MediaFilter/MediaFilter.h"
// using TargetMediaFilter = NS(filter, 1)::TargetMediaFilter;
#include "DataStruct/UnorderedMap.h"
using MediaPlayGroup = UnorderedMap<int, MediaGraphPtr>;
#include "MediaPlay.h"

static int mediaPlayID{ 0 };
static MediaPlayGroup mediaPlayGroup;

int MEDIAPLAY_StartPlayback(
	const char* filePath /* = NULL */, const HWND hwnd /* = NULL */, MEDIAPLAY_VideoFrameCallback callback /* = NULL */)
{
	int status{ ERR_INVALID_PARAM };

	if (filePath && hwnd)
	{
		MediaGraphPtr mediaGraphPtr{ boost::make_shared<FFmpegPlaybackMediaGraph>() };
		if (mediaGraphPtr && ERR_OK == mediaGraphPtr->createNewGraph())
		{
// 			MediaFilterRef videoRendererFilterRef{ mediaGraphPtr->queryMediaFilterByID(NS(filter, 1)::AVMediaVideoRendererFilterID) };
// 			if (!videoRendererFilterRef.expired())
// 			{
// 				MediaModelRef mediaModelRef{ videoRendererFilterRef.lock()->getMediaModel() };
// 				if (!mediaModelRef.expired())
// 				{
// 					boost::shared_ptr<VideoD3DRenderer> videoD3dRendererPtr{
// 						boost::dynamic_pointer_cast<VideoD3DRenderer>(mediaModelRef.lock()) };
// 					if (videoD3dRendererPtr)
// 					{
// 						videoD3dRendererPtr->setVideoDisplayWnd(hwnd);
// 					}
// 				}
// 			}

			framework::multimedia::MediaFilterRef mediaFilterRef{ mediaGraphPtr->queryMediaFilterByID(AVMediaDemuxerFilterID) };
			if (!mediaFilterRef.expired())
			{
				framework::multimedia::MediaModelRef mediaModelRef{ mediaFilterRef.lock()->getMediaModel() };
				if (!mediaModelRef.expired())
				{
					boost::shared_ptr<framework::multimedia::MediaDemuxer> mediaDemuxerPtr{
						boost::dynamic_pointer_cast<framework::multimedia::MediaDemuxer>(mediaModelRef.lock()) };
					if (mediaDemuxerPtr)
					{
						status = mediaDemuxerPtr->openStream(filePath);

						if (ERR_OK == status)
						{
							mediaPlayGroup.insert(++mediaPlayID, mediaGraphPtr);
						}
					}
				}
			}
		}
	}

	return ERR_OK == status ? mediaPlayID : 0;
 }
 
int MEDIAPLAY_StopPlayback(const int playID /* = 0 */)
{
	int status{ 0 };

	if (0 < playID)
	{
		MediaGraphPtr mediaGraphPtr{ mediaPlayGroup.at(playID) };
		if (mediaGraphPtr && ERR_OK == mediaGraphPtr->destroyGraph())
		{
			mediaPlayGroup.remove(playID);
			status = 1;
		}
	}

	return status;
}

int MEDIAPLAY_StartLivestream(
	const char* name /*= NULL*/, const char* password /*= NULL*/,
	const char* address /*= NULL*/, const int port /*= 8000*/,
	const int channel /*= 0*/, const HWND hwnd /*= NULL*/,
	MEDIAPLAY_VideoFrameCallback callback /*= NULL*/,
	void* userData /*= NULL*/)
{
	int status{ ERR_INVALID_PARAM };

// 	if (name && password && address && 0 < port && 0 <= channel && hwnd)
// 	{
// 		++mediaPlayID;
// 		MediaGraphPtr mediaGraphPtr{ boost::make_shared<LivestreamMediaGraph>() };
// 		if (mediaGraphPtr && ERR_OK == mediaGraphPtr->createNewGraph())
// 		{
// 			MediaFilterRef mediaDataCaptureFilterRef{ mediaGraphPtr->queryMediaFilterByID(NS(filter, 1)::AVMediaDataCaptureFilterID) };
// 			if (!mediaDataCaptureFilterRef.expired())
// 			{
// 				boost::shared_ptr<TargetMediaFilter> targetMediaFilterPtr{
// 					boost::dynamic_pointer_cast<TargetMediaFilter>(mediaDataCaptureFilterRef.lock()) };
// 				if (targetMediaFilterPtr)
// 				{
// 					targetMediaFilterPtr->setMediaDataCallback(mediaPlayID, callback, userData);
// 				}
// 			}
// 
// 			MediaFilterRef videoRendererFilterRef{ mediaGraphPtr->queryMediaFilterByID(NS(filter, 1)::AVMediaVideoRendererFilterID) };
// 			if (!videoRendererFilterRef.expired())
// 			{
// 				MediaModelRef mediaModelRef{ videoRendererFilterRef.lock()->getMediaModel() };
// 				if (!mediaModelRef.expired())
// 				{
// 					boost::shared_ptr<VideoD3DRenderer> videoD3dRendererPtr{
// 						boost::dynamic_pointer_cast<VideoD3DRenderer>(mediaModelRef.lock()) };
// 					if (videoD3dRendererPtr)
// 					{
// 						videoD3dRendererPtr->setVideoDisplayWnd(hwnd);
// 					}
// 				}
// 			}
// 
// 			MediaFilterRef mediaFilterRef{ mediaGraphPtr->queryMediaFilterByID(NS(filter, 1)::AVMediaLivestreamCaptureFilterID) };
// 			if (!mediaFilterRef.expired())
// 			{
// 				MediaModelRef mediaModelRef{ mediaFilterRef.lock()->getMediaModel() };
// 				if (!mediaModelRef.expired())
// 				{
// 					boost::shared_ptr<MediaAccessor> mediaAccessorPtr{
// 						boost::dynamic_pointer_cast<MediaAccessor>(mediaModelRef.lock()) };
// 					if (mediaAccessorPtr)
// 					{
// 						status = mediaAccessorPtr->openStream(name, password, address, port, channel, hwnd);
// 
// 						if (ERR_OK == status)
// 						{
// 							mediaPlayGroup.insert(mediaPlayID, mediaGraphPtr);
// 						}
// 					}
// 				}
// 			}
// 		}
// 	}

	return ERR_OK == status ? mediaPlayID : 0;
}

int MEDIAPLAY_StopLivestream(const int playID /* = 0 */)
{
	int status{ 0 };

// 	if (0 < playID)
// 	{
// 		MediaGraphPtr mediaGraphPtr{ mediaPlayGroup.at(playID) };
// 		if (mediaGraphPtr && ERR_OK == mediaGraphPtr->destroyGraph())
// 		{
// 			mediaPlayGroup.remove(playID);
// 			status = 1;
// 		}
// 	}

	return status;
}
