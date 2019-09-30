#ifndef DIGITAL_CAMERA_LIVE_STREAM_H
#define DIGITAL_CAMERA_LIVE_STREAM_H

#include "boost/shared_ptr.hpp"
#include "boost/thread/condition.hpp"
//#include "SDL.h"
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

class Hikvision7xxxNVR;

class DigitCameraLivestream final : public HikvisionLivestream
{
public:
	DigitCameraLivestream(Hikvision7xxxNVR& nvr, const unsigned short idx = -1);
	~DigitCameraLivestream(void);

public:
	int openStream(void) override;
	int closeStream(void) override;
	void setAlgoAbility(const unsigned long long ability = 0);

protected:
	unsigned long long captureJPEGPicture(
		const char* data = NULL, const unsigned long long dataBytes = 0) override;
	void captureVideoStreamProcess(
		const unsigned char* data = NULL, const long long dataBytes = 0, const long dataType = -1) override;
	void captureAudioStreamProcess(
		const unsigned char* data = NULL, const long long dataBytes = 0) override;

private:
	void videoStreamDecodeHandler(
		const char* frameData = NULL, const int dataBytes = 0, const unsigned long long frameNumber = 0,
		const NS(decoder, 1)::DecodeFrame& decodeFrame = NS(decoder, 1)::DecodeFrame::DECODE_FRAME_NONE,
		const int width = 0, const int height = 0);
	void JPEGPFrameEncodeHandler(const char* data = NULL , const int dataBytes = 0 );
//	static int refreshSDLVideo(void* ctx = NULL);
	static DWORD WINAPI frameDecodeProcessThread(void* ctx = NULL);
	BGR24Frame* newBGR24Frame(const BGR24Frame* frame = NULL);
	void deleteBGR24Frame(BGR24Frame* frame);

private:
	Hikvision7xxxNVR& hikvisionNVRDevice;
	boost::shared_ptr<MediaDecoder> videoStreamDecoderPtr;
	boost::shared_ptr<FrameScaler> videoFrameScalerPtr;
	boost::shared_ptr<MediaEncoder> jpegFrameEncoderPtr;
	FIFOQueue helmetBGR24Queue;
	FIFOQueue phoneBGR24Queue;
	FIFOQueue sleepBGR24Queue;
	FIFOQueue fightBGR24Queue;
	FIFOQueue faceBGR24Queue;
	FIFOQueue h264LivestreamQueue;

	unsigned long long livestreamFrameNumber;
	unsigned int algorithmAbility;
	const std::string NVRIp;
	bool stopped;
	//Guarantee work thread exited safely. 
	boost::mutex mtx[3];
	boost::condition condition[3];

// 	SDL_Window* sdlWindow;
// 	SDL_Renderer* sdlRenderer;
// 	SDL_Texture* sdlTexture;
};//class DigitCameraChannel

#endif//DIGITAL_CAMERA_LIVE_STREAM_H

