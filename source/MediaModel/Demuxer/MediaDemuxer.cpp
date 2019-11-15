#include "boost/thread.hpp"
#include "error.h"
#include "MediaModel/Demuxer/MediaDemuxer.h"

NS_BEGIN(model, 1)

MediaDemuxer::MediaDemuxer() : MediaModel(), stopped{ true }
{}

MediaDemuxer::~MediaDemuxer()
{}

int MediaDemuxer::openStream(const std::string streamUrl)
{
	int status{ streamUrl.empty() ? ERR_INVALID_PARAM : ERR_OK };

	if (ERR_OK == status)
	{
		stopped = false;
		notifyMediaDataSubIDCallback(getVideoStreamID(), getAudioStreamID());
		// Start reading data.
		status = runPullStreamDataThread();
	}

	return status;
}

int MediaDemuxer::closeStream(void)
{
	stopped = true;
	// Wait for exiting thread normally.
	boost::unique_lock<boost::mutex> lock{ mtx };
	condition.wait(lock);

	return 0;
}

int MediaDemuxer::runPullStreamDataThread()
{
	int status{ ERR_BAD_OPERATE };

	if (!stopped)
	{
		boost::thread workerThread{ boost::bind(&MediaDemuxer::pullStreamDataWorkerThread, this) };
		workerThread.detach();
		status = ERR_OK;
	}

	return status;
}

void MediaDemuxer::pullStreamDataWorkerThread()
{
	while (!stopped)
	{
		pullStreamDataProcess();
	}

	condition.notify_one();
}

NS_END
