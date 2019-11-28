#include "boost/thread.hpp"
#include "error.h"
#include "MediaModel/Demuxer/MediaDemuxer.h"

namespace framework
{
	namespace multimedia
	{
		MediaDemuxer::MediaDemuxer() : MediaModel(), stopped{ true }
		{}

		MediaDemuxer::~MediaDemuxer()
		{}

		int MediaDemuxer::openStream(const std::string& streamUrl)
		{
			int status{ streamUrl.empty() ? ERR_INVALID_PARAM : ERR_OK };

			if (ERR_OK == status)
			{
				stopped = false;
				boost::thread workerThread{ boost::bind(&MediaDemuxer::mediaDemuxerWorkerThread, this) };
				workerThread.detach();
			}

			return status;
		}

		int MediaDemuxer::closeStream(void)
		{
			stopped = true;
			return ERR_OK;
		}

		int MediaDemuxer::inputMediaData(MediaDataPtr mediaData)
		{
			return ERR_NOT_SUPPORT;
		}
	}//namespace multimedia
}//namespace framework
