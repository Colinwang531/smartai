#include "boost/bind.hpp"
#include "boost/make_shared.hpp"
#include "error.h"
#include "DigitCameraLivestream.h"
using HikvisionSDKDecoder = NS(decoder, 1)::HikvisionSDKDecoder;
using YV12ToBGR24 = NS(scaler, 1)::YV12ToBGR24;
using YV12ToJPEG = NS(encoder, 1)::YV12ToJPEG;

DigitCameraLivestream::DigitCameraLivestream(
	const std::string ip, FIFOList** fqueue /* = NULL */, const int algo /* = 0 */)
	: HikvisionLivestream(), bgr24FrameQueue{ fqueue }, algoMask{ algo }, NVRIp{ ip }
{}

DigitCameraLivestream::~DigitCameraLivestream()
{}

int DigitCameraLivestream::open(const int userID /* = -1 */, const int streamNo /* = -1 */)
{
	int status{ ERR_BAD_ALLOC };
	boost::shared_ptr<MediaDecoder> decoderPtr{ 
		boost::make_shared<HikvisionSDKDecoder>(
			boost::bind(&DigitCameraLivestream::videoStreamDecodeHandler, this, _1, _2, _3, _4, _5)) };
	boost::shared_ptr<FrameScaler> scalerPtr{ boost::make_shared<YV12ToBGR24>() };
	boost::shared_ptr<MediaEncoder> encoderPtr{ boost::make_shared<YV12ToJPEG>() };

	if (decoderPtr && scalerPtr && encoderPtr)
	{
		videoStreamDecoderPtr.swap(decoderPtr);
 		videoFrameScalerPtr.swap(scalerPtr);
		jpegFrameEncoderPtr.swap(encoderPtr);

		status = HikvisionLivestream::open(userID, streamNo) ? ERR_OK : ERR_BAD_OPERATE;
	}

	return status;
}

int DigitCameraLivestream::close()
{
	return HikvisionLivestream::close();
}

void DigitCameraLivestream::captureVideoDataNotifiy(
	const unsigned char* streamData /* = NULL */, const long dataBytes /* = 0 */, const long dataType /* = -1 */)
{
	if (videoStreamDecoderPtr)
	{
		videoStreamDecoderPtr->decode((const char*)streamData, dataBytes);
	}
}

void DigitCameraLivestream::JPEGPFrameEncodeHandler(const char* data /* = NULL */, const int dataBytes /* = 0 */)
{}

// void DigitCameraChannel::audioDataCaptureNotifier(const char* data /* = NULL */, const int dataBytes /* = 0 */)
// {}

void DigitCameraLivestream::videoStreamDecodeHandler(
	const char* data /* = NULL */, const int dataBytes /* = 0 */, 
	const NS(decoder, 1)::DecodeFrame& decodeFrame /* = NS(decoder, 1)::DecodeFrame::DECODE_FRAME_NONE */, 
	const int width /* = 0 */, const int height /* = 0 */)
{
	if (bgr24FrameQueue && videoFrameScalerPtr && jpegFrameEncoderPtr)
	{
		char* jpegFrameData{ NULL };
		int jpegFrameDataBytes{ 0 };
		jpegFrameEncoderPtr->encode(jpegFrameData, jpegFrameDataBytes, data, dataBytes, width, height);
		const int bgr24FrameDataBytes{ width * height * 3 };
		const char* bgr24FrameData{ videoFrameScalerPtr->scale(data, dataBytes, width, height) };

		try
		{
			if (bgr24FrameData)
			{
				int status[5] = { ERR_OK };

				if (algoMask & 0x01)
				{
					//Helmet
					if (!bgr24FrameQueue[0]->isFull())
					{
						BGR24Frame* bgr24Frame = new BGR24Frame;
						bgr24Frame->frameData = new char[bgr24FrameDataBytes];
						bgr24Frame->frameBytes = bgr24FrameDataBytes;
						memcpy_s(bgr24Frame->frameData, bgr24FrameDataBytes, bgr24FrameData, bgr24FrameDataBytes);
						bgr24Frame->jpegData = new char[jpegFrameDataBytes];
						bgr24Frame->jpegBytes = jpegFrameDataBytes;
						memcpy_s(bgr24Frame->jpegData, jpegFrameDataBytes, jpegFrameData, jpegFrameDataBytes);
						bgr24Frame->channelIndex = channelIndex;
						const int iplen{ (const int)NVRIp.length() };
						bgr24Frame->NVRIp = new char[iplen + 1];
						bgr24Frame->NVRIp[iplen] = 0;
						memcpy_s(bgr24Frame->NVRIp, iplen, NVRIp.c_str(), iplen);
						
						if (ERR_OUT_RANGE == bgr24FrameQueue[0]->pushBack(bgr24Frame))
						{
							delete[] bgr24Frame->NVRIp;
							delete[] bgr24Frame->jpegData;
							delete[] bgr24Frame->frameData;
							delete bgr24Frame;
						}
					}
				}
				if ((algoMask >> 1) & 0x01)
				{
					//Phone
					if (!bgr24FrameQueue[1]->isFull())
					{
						BGR24Frame* bgr24Frame = new BGR24Frame;
						bgr24Frame->frameData = new char[bgr24FrameDataBytes];
						bgr24Frame->frameBytes = bgr24FrameDataBytes;
						memcpy_s(bgr24Frame->frameData, bgr24FrameDataBytes, bgr24FrameData, bgr24FrameDataBytes);
						bgr24Frame->jpegData = new char[jpegFrameDataBytes];
						bgr24Frame->jpegBytes = jpegFrameDataBytes;
						memcpy_s(bgr24Frame->jpegData, jpegFrameDataBytes, jpegFrameData, jpegFrameDataBytes);
						bgr24Frame->channelIndex = channelIndex;
						const int iplen{ (const int)NVRIp.length() };
						bgr24Frame->NVRIp = new char[iplen + 1];
						bgr24Frame->NVRIp[iplen] = 0;
						memcpy_s(bgr24Frame->NVRIp, iplen, NVRIp.c_str(), iplen);

						if (ERR_OUT_RANGE == bgr24FrameQueue[1]->pushBack(bgr24Frame))
						{
							delete[] bgr24Frame->NVRIp;
							delete[] bgr24Frame->jpegData;
							delete[] bgr24Frame->frameData;
							delete bgr24Frame;
						}
					}
				}
				if ((algoMask >> 2) & 0x01)
				{
					//Sleep
					if (!bgr24FrameQueue[2]->isFull())
					{
						BGR24Frame* bgr24Frame = new BGR24Frame;
						bgr24Frame->frameData = new char[bgr24FrameDataBytes];
						bgr24Frame->frameBytes = bgr24FrameDataBytes;
						memcpy_s(bgr24Frame->frameData, bgr24FrameDataBytes, bgr24FrameData, bgr24FrameDataBytes);
						bgr24Frame->jpegData = new char[jpegFrameDataBytes];
						bgr24Frame->jpegBytes = jpegFrameDataBytes;
						memcpy_s(bgr24Frame->jpegData, jpegFrameDataBytes, jpegFrameData, jpegFrameDataBytes);
						bgr24Frame->channelIndex = channelIndex;
						const int iplen{ (const int)NVRIp.length() };
						bgr24Frame->NVRIp = new char[iplen + 1];
						bgr24Frame->NVRIp[iplen] = 0;
						memcpy_s(bgr24Frame->NVRIp, iplen, NVRIp.c_str(), iplen);

						if (ERR_OUT_RANGE == bgr24FrameQueue[2]->pushBack(bgr24Frame))
						{
							delete[] bgr24Frame->NVRIp;
							delete[] bgr24Frame->jpegData;
							delete[] bgr24Frame->frameData;
							delete bgr24Frame;
						}
					}
				}
				if ((algoMask >> 3) & 0x01)
				{
					//Fight
					if (!bgr24FrameQueue[3]->isFull())
					{
						BGR24Frame* bgr24Frame = new BGR24Frame;
						bgr24Frame->frameData = new char[bgr24FrameDataBytes];
						bgr24Frame->frameBytes = bgr24FrameDataBytes;
						memcpy_s(bgr24Frame->frameData, bgr24FrameDataBytes, bgr24FrameData, bgr24FrameDataBytes);
						bgr24Frame->jpegData = new char[jpegFrameDataBytes];
						bgr24Frame->jpegBytes = jpegFrameDataBytes;
						memcpy_s(bgr24Frame->jpegData, jpegFrameDataBytes, jpegFrameData, jpegFrameDataBytes);
						bgr24Frame->channelIndex = channelIndex;
						const int iplen{ (const int)NVRIp.length() };
						bgr24Frame->NVRIp = new char[iplen + 1];
						bgr24Frame->NVRIp[iplen] = 0;
						memcpy_s(bgr24Frame->NVRIp, iplen, NVRIp.c_str(), iplen);

						if (ERR_OUT_RANGE == bgr24FrameQueue[3]->pushBack(bgr24Frame))
						{
							delete[] bgr24Frame->NVRIp;
							delete[] bgr24Frame->jpegData;
							delete[] bgr24Frame->frameData;
							delete bgr24Frame;
						}
					}
				}
				if ((algoMask >> 4) & 0x01)
				{
					//Face
					if (!bgr24FrameQueue[4]->isFull())
					{
						BGR24Frame* bgr24Frame = new BGR24Frame;
						bgr24Frame->frameData = new char[bgr24FrameDataBytes];
						bgr24Frame->frameBytes = bgr24FrameDataBytes;
						memcpy_s(bgr24Frame->frameData, bgr24FrameDataBytes, bgr24FrameData, bgr24FrameDataBytes);
						bgr24Frame->jpegData = new char[jpegFrameDataBytes];
						bgr24Frame->jpegBytes = jpegFrameDataBytes;
						memcpy_s(bgr24Frame->jpegData, jpegFrameDataBytes, jpegFrameData, jpegFrameDataBytes);
						bgr24Frame->channelIndex = channelIndex;
						const int iplen{ (const int)NVRIp.length() };
						bgr24Frame->NVRIp = new char[iplen + 1];
						bgr24Frame->NVRIp[iplen] = 0;
						memcpy_s(bgr24Frame->NVRIp, iplen, NVRIp.c_str(), iplen);

						if (ERR_OUT_RANGE == bgr24FrameQueue[4]->pushBack(bgr24Frame))
						{
							delete[] bgr24Frame->NVRIp;
							delete[] bgr24Frame->jpegData;
							delete[] bgr24Frame->frameData;
							delete bgr24Frame;
						}
					}
				}
			}
		}
		catch (std::exception*)
		{}

		if (jpegFrameData)
		{
			delete[] jpegFrameData;
		}
	}
}
