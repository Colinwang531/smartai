#include "boost/format.hpp"
#include "boost/make_shared.hpp"
#include "error.h"
#include "MediaGraph/StandardStreamMediaGraph.h"
using MediaGraphPtr = boost::shared_ptr<framework::multimedia::MediaGraph>;
using StandardStreamMediaGraph = framework::multimedia::StandardStreamMediaGraph;
#include "MediaGraph/SDKStreamMediaGraph.h"
using SDKStreamMediaGraph = framework::multimedia::SDKStreamMediaGraph;
// #include "MediaModel/Accessor/MediaAccessor.h"
// using MediaAccessor = NS(model, 1)::MediaAccessor;
//#include "MediaModel/Demuxer/MediaDemuxer.h"
// using MediaDemuxer = NS(model, 1)::MediaDemuxer;
// #include "MediaModel/Renderer/D3D/VideoD3DRenderer.h"
// using VideoD3DRenderer = NS(model, 1)::VideoD3DRenderer;
#include "MediaFilter/Renderer/AVRendererFilter.h"
using AVRendererFilterPtr = boost::shared_ptr<framework::multimedia::AVRendererFilter>;
using MediaFilterRef = boost::weak_ptr<framework::multimedia::MediaFilter>;
#include "MediaData/MediaData.h"
using MediaData = framework::multimedia::MediaData;
using MediaDataPtr = boost::shared_ptr<MediaData>;
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
		MediaGraphPtr mediaGraphPtr{ boost::make_shared<PlaybackMediaGraph>() };
		if (mediaGraphPtr && ERR_OK == mediaGraphPtr->createNewGraph())
		{
			MediaFilterRef videoRendererFilterRef{ mediaGraphPtr->queryMediaFilterByID(AVMediaVideoRendererFilterID) };
			if (!videoRendererFilterRef.expired())
			{
				AVRendererFilterPtr avrendererFilterPtr{ 
					boost::dynamic_pointer_cast<framework::multimedia::AVRendererFilter>(videoRendererFilterRef.lock()) };
				avrendererFilterPtr->setHwnd(hwnd);
			}

			framework::multimedia::MediaFilterRef mediaFilterRef{ mediaGraphPtr->queryMediaFilterByID(AVMediaDemuxerFilterID) };
			if (!mediaFilterRef.expired())
			{
				MediaDataPtr mediaDataPtr{
					boost::make_shared<MediaData>(
						MediaDataMainID::MEDIA_DATA_MAIN_ID_FILE, MediaDataSubID::MEDIA_DATA_SUB_ID_NONE, MediaDataPatchID::MEDIA_DATA_PATCH_ID_NONE) };
				if (mediaDataPtr)
				{
					mediaDataPtr->setData((const unsigned char*)filePath, (const int)strlen(filePath));
					mediaFilterRef.lock()->inputMediaData(mediaDataPtr);
					mediaPlayGroup.insert(++mediaPlayID, mediaGraphPtr);
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

	if (name && password && address && 0 < port && 0 <= channel && hwnd)
	{
		MediaGraphPtr mediaGraphPtr{ boost::make_shared<LivestreamMediaGraph>() };
		if (mediaGraphPtr && ERR_OK == mediaGraphPtr->createNewGraph())
		{
			MediaFilterRef videoRendererFilterRef{ mediaGraphPtr->queryMediaFilterByID(AVMediaVideoRendererFilterID) };
			if (!videoRendererFilterRef.expired())
			{
				AVRendererFilterPtr avrendererFilterPtr{
					boost::dynamic_pointer_cast<framework::multimedia::AVRendererFilter>(videoRendererFilterRef.lock()) };
				avrendererFilterPtr->setHwnd(hwnd);
			}

			MediaFilterRef mediaFilterRef{ mediaGraphPtr->queryMediaFilterByID(AVMediaLivestreamCaptureFilterID) };
			if (!mediaFilterRef.expired())
			{
				MediaDataPtr mediaDataPtr{
					boost::make_shared<MediaData>(
						MediaDataMainID::MEDIA_DATA_MAIN_ID_STREAM, MediaDataSubID::MEDIA_DATA_SUB_ID_NONE, MediaDataPatchID::MEDIA_DATA_PATCH_ID_NONE) };
				if (mediaDataPtr)
				{
					const std::string streamUrl{ (boost::format("%s:%s:%s:%d:%d") % name % password % address % port % channel).str() };
					mediaDataPtr->setData((const unsigned char*)streamUrl.c_str(), (const int)streamUrl.length());
					mediaFilterRef.lock()->inputMediaData(mediaDataPtr);
					mediaPlayGroup.insert(++mediaPlayID, mediaGraphPtr);
				}
			}
		}
	}

	return ERR_OK == status ? mediaPlayID : 0;
}

int MEDIAPLAY_StopLivestream(const int playID /* = 0 */)
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
