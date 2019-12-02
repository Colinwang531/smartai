#include "boost/thread.hpp"
#include "error.h"
#include "MediaData/MediaData.h"
#include "MediaModel/Demuxer/MediaDemuxer.h"

namespace framework
{
	namespace multimedia
	{
		MediaDemuxer::MediaDemuxer() : MediaModel(), stopped{ true }
		{}

		MediaDemuxer::~MediaDemuxer()
		{}

		int MediaDemuxer::inputMediaData(MediaDataPtr mediaData)
		{
			return mediaData && MediaDataMainID::MEDIA_DATA_MAIN_ID_FILE == mediaData->getMainID() ?
				openStream((const char*)mediaData->getData()) : ERR_INVALID_PARAM;
		}

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
	}//namespace multimedia
}//namespace framework
