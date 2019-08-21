#include "HCNetSDK.h"
#include "error.h"
#include "Stream/Hikvision/HikvisionLivestream.h"

NS_BEGIN(stream, 1)

HikvisionLivestream::HikvisionLivestream() : Livestream(), channelIndex{ -1 }
{}

HikvisionLivestream::~HikvisionLivestream()
{}

int HikvisionLivestream::open(const int userID /* = -1 */, const int streamNo /* = -1 */)
{
	if (ERR_OK == Livestream::open(userID, streamNo))
	{
		NET_DVR_PREVIEWINFO previewInfo{ 0 };
		previewInfo.lChannel = streamNo;
		channelIndex = streamNo;
		livestreamID = NET_DVR_RealPlay_V40(
			userID, &previewInfo, &HikvisionLivestream::livestreamDataCaptureCallback, this);
	}

	return -1 < livestreamID ? ERR_OK : ERR_BAD_OPERATE;
}

int HikvisionLivestream::close()
{
	int status{ Livestream::close() };

	if (ERR_OK == status)
	{
		status = NET_DVR_StopRealPlay(livestreamID) ? ERR_OK : ERR_BAD_OPERATE;
	}

	return status;
}

void HikvisionLivestream::livestreamDataCaptureCallback(
	long livesteamID /* = 0 */, unsigned long dataType /* = 0 */, 
	unsigned char* streamData /* = NULL */, unsigned long dataBytes /* = 0 */, void* ctx /* = NULL */)
{
	HikvisionLivestream* livestream{ reinterpret_cast<HikvisionLivestream*>(ctx) };

	if (livestream)
	{
		if (NET_DVR_SYSHEAD == dataType || NET_DVR_STREAMDATA == dataType)
		{
			livestream->captureVideoDataNotifiy(streamData, dataBytes, dataType);
		}
	}
}

NS_END
