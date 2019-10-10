#include "HCNetSDK.h"
#include "error.h"
#include "Stream/Hikvision/HikvisionLivestream.h"

NS_BEGIN(stream, 1)

HikvisionLivestream::HikvisionLivestream(
	const long uid /* = -1 */, const unsigned short idx /* = -1 */)
	: Livestream(idx), loginUserID{ uid }
{}

HikvisionLivestream::~HikvisionLivestream()
{}

int HikvisionLivestream::openStream()
{
	int status{ ERR_BAD_OPERATE };

	if (-1 < loginUserID && -1 < streamIndex)
	{
		NET_DVR_SetCapturePictureMode(JPEG_MODE);
		NET_DVR_PREVIEWINFO previewInfo{ 0 };
		previewInfo.lChannel = streamIndex;

		streamID = NET_DVR_RealPlay_V40(
			loginUserID, &previewInfo, &HikvisionLivestream::livestreamDataCaptureCallback, this);
		if (-1 < streamID)
		{
			streamStatus = AVStreamStatus::AVSTREAM_STATUS_PLAY;
			status = ERR_OK;
		}
	}

	return status;
}

int HikvisionLivestream::closeStream()
{
	int status{ ERR_BAD_OPERATE };

	if (-1 < streamID && AVStreamStatus::AVSTREAM_STATUS_PLAY == streamStatus)
	{
		if (NET_DVR_StopRealPlay(streamID))
		{
			status = ERR_OK;
			streamID = -1;
			streamStatus = AVStreamStatus::AVSTREAM_STATUS_STOP;

			boost::unique_lock<boost::mutex> lock{ mtx };
			condition.wait_for(lock, boost::chrono::seconds(1));
		}
	}

	return status;
}

unsigned long long HikvisionLivestream::capturePicture(
	const char* data /* = NULL */, const unsigned long long dataBytes /* = 0 */)
{
	return data && 0 < dataBytes && AVStreamStatus::AVSTREAM_STATUS_PLAY == streamStatus ? captureJPEGPicture(data, dataBytes) : 0;
}

unsigned long long HikvisionLivestream::captureJPEGPicture(
	const char* data /*= NULL*/, const unsigned long long dataBytes /*= 0*/)
{
	DWORD capturePictureBytes{ 0 };

	if (-1 < loginUserID && -1 < streamIndex)
	{
		NET_DVR_JPEGPARA jpegParam{ 0xFF, 0 };
		NET_DVR_CaptureJPEGPicture_NEW(
			loginUserID, streamIndex, &jpegParam, const_cast<char*>(data), (unsigned long)dataBytes, &capturePictureBytes);
	}

	return capturePictureBytes;
}

void HikvisionLivestream::livestreamDataCaptureCallback(
	long livestreamID /* = 0 */, unsigned long dataType /* = 0 */, 
	unsigned char* streamData /* = NULL */, unsigned long dataBytes /* = 0 */, void* ctx /* = NULL */)
{
	HikvisionLivestream* livestream{ reinterpret_cast<HikvisionLivestream*>(ctx) };

	if (livestreamID == livestream->streamID)
	{
		if (AVStreamStatus::AVSTREAM_STATUS_STOP == livestream->streamStatus)
		{
			livestream->condition.notify_one();
			return;
		}

		if (NET_DVR_SYSHEAD == dataType || NET_DVR_STREAMDATA == dataType)
		{
			livestream->captureVideoStreamProcess(streamData, dataBytes);
		}
		else if (NET_DVR_AUDIOSTREAMDATA == dataBytes)
		{
			livestream->captureAudioStreamProcess(streamData, dataBytes);
		}
	}
}

NS_END
