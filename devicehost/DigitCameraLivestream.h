#ifndef DIGITAL_CAMERA_LIVE_STREAM_H
#define DIGITAL_CAMERA_LIVE_STREAM_H

#include "boost/shared_ptr.hpp"
#include "boost/thread/condition.hpp"
#include "SDL.h"
#include "DataStruct/FIFOQueue.h"
using FIFOQueue = NS(datastruct, 1)::FIFOQueue<void*>;
#include "Stream/Hikvision/HikvisionLivestream.h"
using HikvisionLivestream = NS(stream, 1)::HikvisionLivestream;
#include "MediaDecoder/SDK/HikvisionSDKDecoder.h"
using MediaDecoder = NS(decoder, 1)::MediaDecoder;
#include "MediaScaler/FFmpeg/YV12ToBGR24.h"
using FrameScaler = NS(scaler, 1)::FrameScaler;
#include "MediaEncoder/FFmpeg/YV12ToJPEG.h"
using MediaEncoder = NS(encoder, 1)::MediaEncoder;

typedef struct tagH264Frame_t
{
	unsigned char* frame;
	long bytes;
}H264Frame;

class DigitCameraLivestream final : public HikvisionLivestream
{
public:
	DigitCameraLivestream(const std::string nvrip, const unsigned short idx = 0);
	~DigitCameraLivestream(void);

public:
	int open(const int userID = -1) override;
	int close(void) override;
	void queue(const int type, std::vector<void*>& out) override;
	void setAlgoAbility(const unsigned int ability = 0);

protected:
	void captureVideoDataNotifiy(
		const unsigned char* streamData = NULL, const long dataBytes = 0, const long dataType = -1) override;
//	void audioDataCaptureNotifier(const char* data = NULL, const int dataBytes = 0) override;

private:
	void videoStreamDecodeHandler(
		const char* frameData = NULL, const int dataBytes = 0, const unsigned long long frameNumber = 0,
		const NS(decoder, 1)::DecodeFrame& decodeFrame = NS(decoder, 1)::DecodeFrame::DECODE_FRAME_NONE,
		const int width = 0, const int height = 0);
	void JPEGPFrameEncodeHandler(const char* data = NULL , const int dataBytes = 0 );
	static int refreshSDLVideo(void* ctx = NULL);
	static DWORD WINAPI frameDecodeProcessThread(void* ctx = NULL);

private:
	boost::shared_ptr<MediaDecoder> videoStreamDecoderPtr;
	boost::shared_ptr<FrameScaler> videoFrameScalerPtr;
	boost::shared_ptr<MediaEncoder> jpegFrameEncoderPtr;
	FIFOQueue helmetBGR24Queue;
	FIFOQueue phoneBGR24Queue;
	FIFOQueue sleepBGR24Queue;
	FIFOQueue fightBGR24Queue;
	FIFOQueue faceBGR24Queue;
	FIFOQueue h264LivestreamQueue;

	unsigned int algorithmAbility;
	const std::string NVRIp;
	bool stopped;
	//Guarantee work thread exited safely. 
	boost::mutex mtx[3];
	boost::condition condition[3];

	SDL_Window* sdlWindow;
	SDL_Renderer* sdlRenderer;
	SDL_Texture* sdlTexture;
};//class DigitCameraChannel

#endif//DIGITAL_CAMERA_LIVE_STREAM_H

