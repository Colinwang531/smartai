#include "error.h"
#include "MediaGraph/FileMediaGraph.h"

NS_BEGIN(graph, 1)

FileMediaGraph::FileMediaGraph() : MediaGraph()
{}

FileMediaGraph::~FileMediaGraph()
{}

int FileMediaGraph::play()
{
	return ERR_OK;
}

int FileMediaGraph::stop()
{
	return ERR_OK;
}

int FileMediaGraph::pause()
{
	return ERR_OK;
}

int FileMediaGraph::fast(const unsigned short speed /* = 1 */)
{
	return checkSpeedValue(speed);
}

int FileMediaGraph::slow(const unsigned short speed /* = 1 */)
{
	return checkSpeedValue(speed);
}

int FileMediaGraph::checkSpeedValue(const unsigned short speed /* = 1 */)
{
	return 1 == speed || 2 == speed || 4 == speed || 8 == speed || 16 == speed ?
		ERR_OK : ERR_INVALID_PARAM;
}

NS_END
