#include "error.h"
#include "MediaGraph/MediaGraph.h"

NS_BEGIN(graph, 1)

MediaGraph::MediaGraph()
{}

MediaGraph::~MediaGraph()
{}

MediaFilterRef MediaGraph::queryMediaFilterByID(const std::string filterID)
{
	return mediaFilterGroup.at(filterID);
}

int MediaGraph::addMediaFilter(const std::string filterID, MediaFilterPtr mediaFilter)
{
	int status{ !filterID.empty() && mediaFilter ? ERR_OK : ERR_INVALID_PARAM };

	if (ERR_OK == status)
	{
		mediaFilterGroup.insert(filterID, mediaFilter);
	}

	return status;
}

int MediaGraph::removeMediaFilter(const std::string filterID)
{
	int status{ filterID.empty() ? ERR_INVALID_PARAM : ERR_OK };

	if (ERR_OK == status)
	{
		mediaFilterGroup.remove(filterID);
	}

	return status;
}

NS_END
