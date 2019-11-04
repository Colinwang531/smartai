extern "C"
{
#include "libavutil/pixdesc.h"
}
#include "error.h"
#include "MediaDemuxer/FFmpeg/FFmpegFileMediaDemuxer.h"

NS_BEGIN(demuxer, 1)

FFmpegFileMediaDemuxer::FFmpegFileMediaDemuxer() : MediaDemuxer(), avFormatContext{ NULL }
{}

FFmpegFileMediaDemuxer::~FFmpegFileMediaDemuxer(void)
{
	if (avFormatContext)
	{
		avformat_close_input((AVFormatContext**)&avFormatContext);
	}
}

int FFmpegFileMediaDemuxer::inputMediaData(MediaDataPtr mediaData)
{
	int status{ ERR_OK };
	boost::weak_ptr<char[]> filePathRef{ mediaData->getMediaDataBuffer() };

	if (!filePathRef.expired())
	{
		if (!avformat_open_input((AVFormatContext**)&avFormatContext, filePathRef.lock().get(), NULL, NULL))
		{
			status = (0 <= avformat_find_stream_info((AVFormatContext*)avFormatContext, NULL) ? ERR_OK : ERR_STREAM_TYPE_UNKNOWN);
		}
		else
		{
			status = ERR_FILE_OPEN_FAILED;
		}
	}

	return 0;
}

NS_END
