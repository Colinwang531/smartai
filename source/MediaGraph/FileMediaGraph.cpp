#include "error.h"
#include "MediaGraph/FileMediaGraph.h"

NS_BEGIN(graph, 1)

FileMediaGraph::FileMediaGraph() : MediaGraph()
{}

FileMediaGraph::~FileMediaGraph()
{}

int FileMediaGraph::openFile(const char* filePath /*= NULL*/)
{
	return filePath && 0 < strlen(filePath) ? ERR_OK : ERR_INVALID_PARAM;
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

int FileMediaGraph::slowPlay(const short speed /* = 1 */)
{
	return checkSpeedValue(speed);
}

int FileMediaGraph::checkSpeedValue(const short speed /* = 1 */)
{
	return 1 == speed || 2 == speed || 4 == speed || 8 == speed || 16 == speed ?
		ERR_OK : ERR_INVALID_PARAM;
}

NS_END
