#ifndef DIGITAL_CAMERA_LIVE_STREAM_H
#define DIGITAL_CAMERA_LIVE_STREAM_H

#include "boost/shared_ptr.hpp"
// #include "boost/function.hpp"
#include "Stream/Hikvision/HikvisionLivestream.h"
using HikvisionLivestream = NS(stream, 1)::HikvisionLivestream;
#include "MediaDecoder/SDK/HikvisionSDKDecoder.h"
using MediaDecoder = NS(decoder, 1)::MediaDecoder;
// using HikvisionSDKDecoder = NS(decoder, 1)::HikvisionSDKDecoder;
#include "MediaScaler/FFmpeg/YV12ToBGR24.h"
using FrameScaler = NS(scaler, 1)::FrameScaler;
#include "MediaEncoder/FFmpeg/YV12ToJPEG.h"
using MediaEncoder = NS(encoder, 1)::MediaEncoder;
// #include "MQModel/PublisherServerModel.h"
// using PublisherServerModel = NS(model, 1)::PublisherServerModel;
// #include "MediaRenderer/SDLRenderer.h"
// using MediaRenderer = NS(renderer, 1)::MediaRenderer;
// using SDLRenderer = NS(renderer, 1)::SDLRenderer;
#include "BGR24FrameCache.h"

//typedef boost::function<void(const LONG, const char*, const int, const char*, const int)> LivestreamFrameCallback;

class DigitCameraLivestream final : public HikvisionLivestream
{
public:
	DigitCameraLivestream(BGR24FrameCache* cache = NULL);
	~DigitCameraLivestream(void);

public:
	int open(const int userID = -1, const int streamNo = -1) override;
	int close(void) override;

protected:
	void captureVideoDataNotifiy(
		const unsigned char* streamData = NULL, const long dataBytes = 0, const long dataType = -1) override;
//	void audioDataCaptureNotifier(const char* data = NULL, const int dataBytes = 0) override;

private:
	void videoStreamDecodeHandler(
		const char* data = NULL, const int dataBytes = 0, 
		const NS(decoder, 1)::DecodeFrame& decodeFrame = NS(decoder, 1)::DecodeFrame::DECODE_FRAME_NONE,
		const int width = 0, const int height = 0);
	void JPEGPFrameEncodeHandler(const char* data = NULL , const int dataBytes = 0 );
//	void processAlgoResult(const char* data = NULL, const int pixelWidth = 1920, const int pixelHeight = 1080);

private:
	boost::shared_ptr<MediaDecoder> videoStreamDecoderPtr;
	boost::shared_ptr<FrameScaler> videoFrameScalerPtr;
	boost::shared_ptr<MediaEncoder> jpegFrameEncoderPtr;
// 	LivestreamFrameCallback livestreamFrameCallback;
	BGR24FrameCache* bgr24FrameCache;
};//class DigitCameraChannel

#endif//DIGITAL_CAMERA_LIVE_STREAM_H

