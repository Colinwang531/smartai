#include "boost/make_shared.hpp"
#include "error.h"
#include "MediaGraph/FFmpeg/FFmpegPlaybackStreamMediaGraph.h"
using MediaGraphPtr = boost::shared_ptr<NS(graph, 1)::MediaGraph>;
using FFmpegPlaybackStreamMediaGraph = NS(graph, 1)::FFmpegPlaybackStreamMediaGraph;
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
		MediaGraphPtr mediaGraphPtr{ boost::make_shared<FFmpegPlaybackStreamMediaGraph>() };
		if (mediaGraphPtr && ERR_OK == mediaGraphPtr->openMediaGraph(url, hwnd))
		{
			mediaPlayGroup.insert(++mediaPlayID, mediaGraphPtr);
			status = ERR_OK;
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
		if (mediaGraphPtr && ERR_OK == mediaGraphPtr->closeMediaGraph())
		{
			mediaPlayGroup.remove(playID);
			status = 1;
		}
	}

	return status;
}
