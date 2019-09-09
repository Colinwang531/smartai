#ifndef DIGITAL_CAMERA_LIVE_STREAM_H
#define DIGITAL_CAMERA_LIVE_STREAM_H

#include "boost/shared_ptr.hpp"
#include "boost/thread/condition.hpp"
#include "DataStruct/FIFOList.h"
using FIFOList = NS(datastruct, 1)::FIFOList;
#include "Stream/Hikvision/HikvisionLivestream.h"
using HikvisionLivestream = NS(stream, 1)::HikvisionLivestream;
#include "MediaDecoder/SDK/HikvisionSDKDecoder.h"
using MediaDecoder = NS(decoder, 1)::MediaDecoder;
#include "MediaScaler/FFmpeg/YV12ToBGR24.h"
using FrameScaler = NS(scaler, 1)::FrameScaler;
#include "MediaEncoder/FFmpeg/YV12ToJPEG.h"
using MediaEncoder = NS(encoder, 1)::MediaEncoder;

class DigitCameraLivestream final : public HikvisionLivestream
{
public:
	DigitCameraLivestream(
		const std::string ip, FIFOList** fqueue = NULL, const int algo = 0);
	~DigitCameraLivestream(void);

public:
	int open(const int userID = -1, const int streamNo = -1) override;
	int close(void) override;
	void modifyAlgoMask(const int mask = 0);

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

private:
	boost::shared_ptr<MediaDecoder> videoStreamDecoderPtr;
	boost::shared_ptr<FrameScaler> videoFrameScalerPtr;
	boost::shared_ptr<MediaEncoder> jpegFrameEncoderPtr;
	FIFOList** bgr24FrameQueue;
	int algoMask;
	const std::string NVRIp;

	bool stopped;
	//Guarantee work thread exited safely. 
	boost::mutex mtx[2];
	boost::condition condition[2];
};//class DigitCameraChannel

#endif//DIGITAL_CAMERA_LIVE_STREAM_H

