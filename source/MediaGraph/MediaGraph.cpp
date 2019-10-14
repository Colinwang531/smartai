#include "error.h"
#include "MediaGraph/MediaGraph.h"

NS_BEGIN(graph, 1)

MediaGraph::MediaGraph()
{}

MediaGraph::~MediaGraph()
{}

int MediaGraph::addFilter(const std::string filterID, MediaFilterPtr filterPtr)
{
	return filterID.empty() || !filterPtr ? ERR_INVALID_PARAM : mediaFilterGroup.insert(filterID, filterPtr);
}

int MediaGraph::removeFilter(const std::string filterID)
{
	return filterID.empty() ? ERR_INVALID_PARAM : mediaFilterGroup.remove(filterID);
}

NS_END
