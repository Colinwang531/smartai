#include "error.h"
#include "MediaFilter/MediaFilter.h"
#include "MediaGraph/File/LocalFileMediaGraph.h"

NS_BEGIN(graph, 1)

LocalFileMediaGraph::LocalFileMediaGraph() : FileMediaGraph()
{}

LocalFileMediaGraph::~LocalFileMediaGraph()
{}

int LocalFileMediaGraph::openFile(const char* filePath /*= NULL*/)
{
	int status{ FileMediaGraph::openFile(filePath) };

	if (ERR_OK == status)
	{
	}

	return status;
}

int LocalFileMediaGraph::closeFile(void)
{
	return 0;
}

NS_END
