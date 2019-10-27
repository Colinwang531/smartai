#include "error.h"
#include "MediaGraph/StreamMediaGraph.h"

NS_BEGIN(graph, 1)

StreamMediaGraph::StreamMediaGraph() : MediaGraph()
{}

StreamMediaGraph::~StreamMediaGraph()
{}

int StreamMediaGraph::startMediaGraph(const std::string streamUrl)
{
	return streamUrl.empty() ? ERR_INVALID_PARAM : openStream(streamUrl);
}

int StreamMediaGraph::stopMediaGraph()
{
	return ERR_OK;
}

NS_END
