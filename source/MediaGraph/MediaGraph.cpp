#include "error.h"
#include "MediaFilter/MediaFilter.h"
#include "MediaGraph/MediaGraph.h"

namespace framework
{
	namespace multimedia
	{
		MediaGraph::MediaGraph()
		{}

		MediaGraph::~MediaGraph()
		{}

		MediaFilter* MediaGraph::queryMediaFilterByID(const std::string& filterID)
		{
			return mediaFilterGroup.at(filterID);
		}

		int MediaGraph::addMediaFilter(const std::string& filterID, MediaFilter* filter /* = NULL */)
		{
			int status{ filterID.empty() || filter ? ERR_INVALID_PARAM : ERR_OK };

			if (ERR_OK == status)
			{
				mediaFilterGroup.insert(filterID, filter);
			}

			return status;
		}

		int MediaGraph::removeMediaFilter(const std::string& filterID)
		{
			int status{ filterID.empty() ? ERR_INVALID_PARAM : ERR_OK };

			if (ERR_OK == status)
			{
				mediaFilterGroup.remove(filterID);
			}

			return status;
		}
	}//namespace multimedia
}//namespace framework
