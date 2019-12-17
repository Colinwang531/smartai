#include "boost/bind.hpp"
#include "boost/make_shared.hpp"
#include "boost/thread/mutex.hpp"
#include "error.h"
#include "MediaGraph/MediaGraph.h"
using MediaGraphPtr = boost::shared_ptr<framework::multimedia::MediaGraph>;
// #include "MediaModel/Accessor/MediaAccessor.h"
// using MediaAccessor = NS(model, 1)::MediaAccessor;
//#include "MediaModel/Demuxer/MediaDemuxer.h"
// using MediaDemuxer = NS(model, 1)::MediaDemuxer;
// #include "MediaModel/Renderer/D3D/VideoD3DRenderer.h"
// using VideoD3DRenderer = NS(model, 1)::VideoD3DRenderer;
// #include "MediaFilter/Renderer/AVRendererFilter.h"
// using AVRendererFilterPtr = boost::shared_ptr<framework::multimedia::AVRendererFilter>;
// using MediaFilterRef = boost::weak_ptr<framework::multimedia::MediaFilter>;
// #include "MediaData/MediaData.h"
// using MediaData = framework::multimedia::MediaData;
// using MediaDataPtr = boost::shared_ptr<MediaData>;
#include "DataStruct/UnorderedMap.h"
using MediaPlayGroup = UnorderedMap<const int, MediaGraphPtr>;
#include "MediaPlay.h"

static boost::mutex mtx;
static int mediaPlayID{ 0 };
static MediaPlayGroup mediaPlayGroup;

int MEDIAPLAY_OpenStream(
	const char* url /* = NULL */, const HWND hwnd /* = NULL */, MEDIAPLAY_MediaFrameCallback callback /* = NULL */, void* userData /* = NULL */)
{
	int status{ ERR_INVALID_PARAM };

	if (url)
	{
		MediaGraphPtr mediaGraphPtr{ boost::make_shared<framework::multimedia::MediaGraph>() };
		if (mediaGraphPtr)
		{
			status = mediaGraphPtr->openStream(url, hwnd, boost::bind(callback, _1, _2, _3, _4, _5), userData);
			if (ERR_OK == status)
			{
				mtx.lock();
				++mediaPlayID;
				mtx.unlock();
				mediaPlayGroup.insert(mediaPlayID, mediaGraphPtr);
			}
		}
	}

	return ERR_OK == status ? mediaPlayID : 0;
}

int MEDIAPLAY_CloseStream(const int playID /* = 0 */)
{
	int status{ ERR_NOT_FOUND };

	if (0 < playID)
	{
		MediaGraphPtr mediaGraphPtr{ mediaPlayGroup.at(playID) };
		if (mediaGraphPtr)
		{
			status = mediaGraphPtr->closeStream();
			if (ERR_OK == status)
			{
				mediaPlayGroup.remove(playID);
			}
		}
	}

	return status;
}
