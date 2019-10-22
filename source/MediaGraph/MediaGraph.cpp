#include "error.h"
#include "MediaGraph/MediaGraph.h"

NS_BEGIN(graph, 1)

MediaGraph::MediaGraph()
{}

MediaGraph::~MediaGraph()
{}

MediaFilterRef MediaGraph::queryFilterByID(const std::string filterID)
{
	return mediaFilterGroup.at(filterID);
}

int MediaGraph::addFilter(const std::string filterID, MediaFilterPtr filterPtr)
{
	int status{ !filterID.empty() && filterPtr ? ERR_OK : ERR_INVALID_PARAM };

	if (ERR_OK == status)
	{
		mediaFilterGroup.insert(filterID, filterPtr);
	}

	return status;
}

int MediaGraph::removeFilter(const std::string filterID)
{
	int status{ filterID.empty() ? ERR_INVALID_PARAM : ERR_OK };

	if (ERR_OK == status)
	{
		mediaFilterGroup.remove(filterID);
	}

	return status;
}

NS_END
