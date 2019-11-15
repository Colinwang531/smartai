#include "boost/make_shared.hpp"
#include "error.h"
#include "MediaGraph/File/LocalFilePlaybackMediaGraph.h"
using MediaGraphPtr = boost::shared_ptr<NS(graph, 1)::MediaGraph>;
using LocalFilePlaybackMediaGraph = NS(graph, 1)::LocalFilePlaybackMediaGraph;
#include "MediaModel/Demuxer/MediaDemuxer.h"
using MediaDemuxer = NS(model, 1)::MediaDemuxer;
#include "DataStruct/UnorderedMap.h"
using MediaPlayGroup = UnorderedMap<int, MediaGraphPtr>;
#include "MediaPlay.h"

static int mediaPlayID{ 0 };
static MediaPlayGroup mediaPlayGroup;

int MEDIAPLAY_StartPlay(
	const char* url /* = NULL */, const HWND hwnd /* = NULL */, MEDIAPLAY_VideoFrameCallback callback /* = NULL */)
{
	int status{ ERR_INVALID_PARAM };

	if (url && hwnd)
	{
		MediaGraphPtr mediaGraphPtr{ boost::make_shared<LocalFilePlaybackMediaGraph>() };
		if (mediaGraphPtr && ERR_OK == mediaGraphPtr->createNewGraph(hwnd, callback))
		{
			MediaFilterRef mediaFilterRef{ mediaGraphPtr->queryMediaFilterByID(NS(filter, 1)::AVMediaDemuxerFilterID) };
			if (!mediaFilterRef.expired())
			{
				MediaModelRef mediaModelRef{ mediaFilterRef.lock()->getMediaModel() };
				if (!mediaModelRef.expired())
				{
					boost::shared_ptr<MediaDemuxer> mediaDemuxerPtr{ 
						boost::dynamic_pointer_cast<MediaDemuxer>(mediaModelRef.lock()) };
					if (mediaDemuxerPtr)
					{
						status = mediaDemuxerPtr->openStream(url);

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
 
int MEDIAPLAY_StopPlay(const int playID /* = 0 */)
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
