#include "error.h"
#include "MediaGraph/FileMediaGraph.h"

NS_BEGIN(graph, 1)

FileMediaGraph::FileMediaGraph() : MediaGraph()
{}

FileMediaGraph::~FileMediaGraph()
{}

int FileMediaGraph::startMediaGraph(const std::string filePath)
{
	int status{ filePath.empty() ? ERR_INVALID_PARAM : openFile(filePath) };

	if (ERR_OK == status)
	{
		status = buildGraph();
	}

	return status;
}

int FileMediaGraph::stopMediaGraph()
{
	return ERR_OK;
}

int FileMediaGraph::startPlay()
{
	return ERR_OK;
}

int FileMediaGraph::stopPlay()
{
	return ERR_OK;
}

int FileMediaGraph::pausePlay()
{
	return ERR_OK;
}

int FileMediaGraph::fastPlay(const short speed /* = 1 */)
{
	return checkSpeedValue(speed);
}

int FileMediaGraph::slowPlay(const short speed /* = -1 */)
{
	return checkSpeedValue(speed);
}

int FileMediaGraph::checkSpeedValue(const short speed /* = 1 */)
{
	return 1 == speed || 2 == speed || 4 == speed || 8 == speed || 16 == speed ||
		-1 == speed || -2 == speed || -4 == speed || -8 == speed || -16 == speed ?
		ERR_OK : ERR_INVALID_PARAM;
}

NS_END
