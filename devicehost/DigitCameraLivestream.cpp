#include "boost/bind.hpp"
#include "boost/make_shared.hpp"
#include "error.h"
#include "DigitCameraLivestream.h"
using HikvisionSDKDecoder = NS(decoder, 1)::HikvisionSDKDecoder;
using YV12ToBGR24 = NS(scaler, 1)::YV12ToBGR24;
using YV12ToJPEG = NS(encoder, 1)::YV12ToJPEG;

DigitCameraLivestream::DigitCameraLivestream(BGR24FrameCache* cache /* = NULL */, const int algo /* = 0 */)
	: HikvisionLivestream(), bgr24FrameCache{ cache }, algoMask{ algo }
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
	if (bgr24FrameCache && videoFrameScalerPtr && jpegFrameEncoderPtr)
	{
		char* jpegFrameData{ NULL };
		int jpegFrameDataBytes{ 0 };
		jpegFrameEncoderPtr->encode(jpegFrameData, jpegFrameDataBytes, data, dataBytes, width, height);
		const int bgr24FrameDataBytes{ width * height * 3 };
		const char* bgr24FrameData{ videoFrameScalerPtr->scale(data, dataBytes, width, height) };

		if (bgr24FrameData)
		{
			if (algoMask & 0x01)
			{
				bgr24FrameCache[0].insert(bgr24FrameData, bgr24FrameDataBytes, jpegFrameData, jpegFrameDataBytes);
			}
			if ((algoMask >> 1) & 0x01)
			{
				bgr24FrameCache[1].insert(bgr24FrameData, bgr24FrameDataBytes, jpegFrameData, jpegFrameDataBytes);
			}
			if ((algoMask >> 2) & 0x01)
			{
				bgr24FrameCache[2].insert(bgr24FrameData, bgr24FrameDataBytes, jpegFrameData, jpegFrameDataBytes);
			}
			if ((algoMask >> 3) & 0x01)
			{
				bgr24FrameCache[3].insert(bgr24FrameData, bgr24FrameDataBytes, jpegFrameData, jpegFrameDataBytes);
			}
			if ((algoMask >> 4) & 0x01)
			{
				bgr24FrameCache[4].insert(bgr24FrameData, bgr24FrameDataBytes, jpegFrameData, jpegFrameDataBytes);
			}
		}

		if (jpegFrameData)
		{
			delete[] jpegFrameData;
		}
	}
}
