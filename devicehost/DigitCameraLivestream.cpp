// #include <fstream>
// #include <io.h>
#include "boost/bind.hpp"
#include "boost/make_shared.hpp"
// #include "AlarmMessage.h"
#include "error.h"
#include "DigitCameraLivestream.h"
using HikvisionSDKDecoder = NS(decoder, 1)::HikvisionSDKDecoder;
using YV12ToBGR24 = NS(scaler, 1)::YV12ToBGR24;
using YV12ToJPEG = NS(encoder, 1)::YV12ToJPEG;

DigitCameraLivestream::DigitCameraLivestream(BGR24FrameCache* cache /* = NULL */)
	: HikvisionLivestream(), bgr24FrameCache{ cache }
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
// 	jpegPictureDataBuffer = new(std::nothrow) char[2 * 1024 * 1024];

	if (decoderPtr && scalerPtr && encoderPtr)
	{
		videoStreamDecoderPtr.swap(decoderPtr);
// 		status = realplayDataDecoder->initialize();
 		videoFrameScalerPtr.swap(scalerPtr);
		jpegFrameEncoderPtr.swap(encoderPtr);
// 		status = frameDataScaler->initialize();

		status = HikvisionLivestream::open(userID, streamNo) ? ERR_OK : ERR_BAD_OPERATE;
	}

// 	boost::shared_ptr<MediaRenderer> renderer{ boost::make_shared<SDLRenderer>() };
// 	if (ERR_OK == status && renderer)
// 	{
// 		sdlVideoRendererPtr.swap(renderer);
// 		sdlVideoRendererPtr->createRenderer();
// 	}

//	return ERR_OK == status ? HIKRealplayChannel::openChannel(channelNumber) : status;

	return status;
}

int DigitCameraLivestream::close()
{
// 	int status{ HIKRealplayChannel::closeChannel() };
// 
// 	if (ERR_OK == status && realplayDataDecoder && frameDataScaler)
// 	{
// 		realplayDataDecoder->uninitialize();
// 		frameDataScaler->uninitialize();
// 	}
// 
// 	if (sdlVideoRendererPtr)
// 	{
// 		sdlVideoRendererPtr->destroyRenderer();
// 	}
// 
// 	if (jpegPictureDataBuffer)
// 	{
// 		delete[] jpegPictureDataBuffer;
// 		jpegPictureDataBuffer = NULL;
// 	}
// 
// 	return status;

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
{
// 	if (data && 0 < dataBytes && jpegPictureDataBuffer)
// 	{
// 		memcpy_s(jpegPictureDataBuffer, dataBytes, data, dataBytes);
// 		jpegPictureDataBytes = dataBytes;
// 	}
}

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
//			NS(scaler, 1)::EncodeYUVToJPEG2(data, dataBytes, pixelWidth, pixelHeight);
//			sdlVideoRendererPtr->pushData(scaleFrameData);

// 			if (livestreamFrameCallback)
// 			{
// 				livestreamFrameCallback(realplayID, scaleFrameData, frameDataBytes, jpegPictureDataBuffer, jpegPictureDataBytes);
// 			}

			for (int i = 0; i != 3; ++i)
			{
				bgr24FrameCache[i].insert(bgr24FrameData, bgr24FrameDataBytes, jpegFrameData, jpegFrameDataBytes);
			}
		}

		if (jpegFrameData)
		{
			delete[] jpegFrameData;
		}
	}
}
