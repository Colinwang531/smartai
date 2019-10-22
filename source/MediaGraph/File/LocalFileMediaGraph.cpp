#include "error.h"
#include "MediaGraph/File/LocalFileMediaGraph.h"

NS_BEGIN(graph, 1)

LocalFileMediaGraph::LocalFileMediaGraph() : FileMediaGraph()
{}

LocalFileMediaGraph::~LocalFileMediaGraph()
{}

int LocalFileMediaGraph::openFile(const std::string filePath)
{
	return ERR_OK;
}

int LocalFileMediaGraph::closeFile(void)
{
	return ERR_OK;
}

NS_END
