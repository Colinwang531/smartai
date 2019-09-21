#include "HCNetSDK.h"
#include "error.h"
#include "Stream/Hikvision/HikvisionLivestream.h"

NS_BEGIN(stream, 1)

HikvisionLivestream::HikvisionLivestream(const unsigned short idx /* = 0 */)
	: Livestream(idx)
{}

HikvisionLivestream::~HikvisionLivestream()
{}

int HikvisionLivestream::open(const int userID /* = -1 */)
{
	if (-1 < userID && -1 < cameraIndex)
	{
		NET_DVR_SetCapturePictureMode(JPEG_MODE);
		NET_DVR_PREVIEWINFO previewInfo{ 0 };
		previewInfo.lChannel = cameraIndex;
		livestreamID = NET_DVR_RealPlay_V40(userID, &previewInfo, &HikvisionLivestream::livestreamDataCaptureCallback, this);
	}

	return -1 < livestreamID ? ERR_OK : ERR_BAD_OPERATE;
}

int HikvisionLivestream::close()
{
	int status{ ERR_BAD_OPERATE };

	if (-1 < livestreamID)
	{
		status = NET_DVR_StopRealPlay(livestreamID) ? ERR_OK : ERR_BAD_OPERATE;
	}

	return status;
}

int HikvisionLivestream::capture(
	const int userID, const int cameraIndex, char*& jpegData, const int jpegBytes/* = 1024 * 1024*/)
{
	int status{ 0 };

	if (-1 < userID && -1 < cameraIndex && jpegData && 0 < jpegBytes)
	{
		DWORD captureDataBytes{ 0 };
		NET_DVR_JPEGPARA jpegParam{ 0xFF, 0 };
		if (NET_DVR_CaptureJPEGPicture_NEW(userID, cameraIndex, &jpegParam, jpegData, jpegBytes, &captureDataBytes))
		{
			status = (int)captureDataBytes;
		}
	}
	
	return status;
}

void HikvisionLivestream::livestreamDataCaptureCallback(
	long livesteamID /* = 0 */, unsigned long dataType /* = 0 */, 
	unsigned char* streamData /* = NULL */, unsigned long dataBytes /* = 0 */, void* ctx /* = NULL */)
{
	HikvisionLivestream* livestream{ reinterpret_cast<HikvisionLivestream*>(ctx) };

	if (livestream && -1 < livestream->livestreamID && -1 < livestream->cameraIndex)
	{
		if (NET_DVR_SYSHEAD == dataType || NET_DVR_STREAMDATA == dataType)
		{
			livestream->captureVideoDataNotifiy(streamData, dataBytes, dataType);
		}
	}
}

NS_END
