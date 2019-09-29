#include "boost/bind.hpp"
#include "boost/make_shared.hpp"
#include "error.h"
#include "DigitCameraLivestream.h"
using HikvisionSDKDecoder = NS(decoder, 1)::HikvisionSDKDecoder;
using YV12ToBGR24 = NS(scaler, 1)::YV12ToBGR24;
using YV12ToJPEG = NS(encoder, 1)::YV12ToJPEG;

extern int sailingStatus;//0 : sail, 1 : port
extern int autoCheckSwitch;//0 : manual, 1 : auto
//unsigned char covertBuffer[1920 * 1080 * 4];

// void c24To32(unsigned char* src, unsigned char* dest, int w, int h)
// {
// 	for (int i = 0; i < h; i++)
// 	{
// 		for (int j = 0; j < w; j++)
// 		{
// 			if (SDL_BYTEORDER == SDL_LIL_ENDIAN)
// 			{
// 				dest[(i * w + j) * 4 + 0] = src[(i * w + j) * 3 + 2];
// 				dest[(i * w + j) * 4 + 1] = src[(i * w + j) * 3 + 1];
// 				dest[(i * w + j) * 4 + 2] = src[(i * w + j) * 3];
// 				dest[(i * w + j) * 4 + 3] = 0;
// 			}
// 			else
// 			{
// 				dest[(i * w + j) * 4] = 0;
// 				memcpy_s(dest + (i * w + j) * 4 + 1, 3, src + (i * w + j) * 3, 3);
// 			}
// 		}
// 	}
// }

DigitCameraLivestream::DigitCameraLivestream(const std::string nvrip, const unsigned short idx /* = 0 */)
	: HikvisionLivestream(idx), algorithmAbility{ 0 }, NVRIp{ nvrip }, stopped{ false }, livestreamFrameNumber{ 0 }//,
//	sdlWindow{ NULL }, sdlRenderer{ NULL }, sdlTexture{ NULL }
{}

DigitCameraLivestream::~DigitCameraLivestream()
{}

int DigitCameraLivestream::open(const int userID /* = -1 */)
{
	int status{ ERR_BAD_ALLOC };
	boost::shared_ptr<MediaDecoder> decoderPtr{ 
		boost::make_shared<HikvisionSDKDecoder>(
			boost::bind(&DigitCameraLivestream::videoStreamDecodeHandler, this, _1, _2, _3, _4, _5, _6)) };
	boost::shared_ptr<FrameScaler> scalerPtr{ boost::make_shared<YV12ToBGR24>() };
	boost::shared_ptr<MediaEncoder> encoderPtr{ boost::make_shared<YV12ToJPEG>() };

	if (decoderPtr && scalerPtr && encoderPtr)
	{
		videoStreamDecoderPtr.swap(decoderPtr);
 		videoFrameScalerPtr.swap(scalerPtr);
		jpegFrameEncoderPtr.swap(encoderPtr);
		helmetBGR24Queue.setCapacity(12);
		phoneBGR24Queue.setCapacity(16);
		sleepBGR24Queue.setCapacity(16);
		fightBGR24Queue.setCapacity(16);
		faceBGR24Queue.setCapacity(16);
		h264LivestreamQueue.setCapacity(1000);
		DWORD threadID{ 0 };
		CreateThread(NULL, 0, &DigitCameraLivestream::frameDecodeProcessThread, this, 0, &threadID);

		status = HikvisionLivestream::open(userID) ? ERR_OK : ERR_BAD_OPERATE;

// 		SDL_Init(SDL_INIT_VIDEO);
// 		sdlWindow = SDL_CreateWindow("SDL Player", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 960, 540, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
// 		if (sdlWindow)
// 		{
// 			sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, 0);
// 			sdlTexture = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_BGR888, SDL_TEXTUREACCESS_STREAMING, 1920, 1080);
// 			SDL_CreateThread(&DigitCameraLivestream::refreshSDLVideo, NULL, NULL);
// 		}
	}

	return status;
}

int DigitCameraLivestream::close()
{
	int status{ HikvisionLivestream::close() };
	videoStreamDecoderPtr.reset();

	if (ERR_OK == status)
	{
		stopped = true;
		boost::unique_lock<boost::mutex> lock1{ mtx[0] }, lock2{ mtx[1] }, lock3{ mtx[2] };
		condition[0].wait_for(lock1, boost::chrono::seconds(1));
		condition[1].wait_for(lock2, boost::chrono::seconds(1));
		condition[2].wait_for(lock3, boost::chrono::seconds(1));
	}

	return status;
}


void DigitCameraLivestream::setAlgoAbility(const unsigned int ability /* = 0 */)
{
	algorithmAbility = ability;
}

void DigitCameraLivestream::queue(const int type, std::vector<void*>& out)
{
	if (0 == type)
	{
		helmetBGR24Queue.swap(out);
	} 
	else if (1 == type)
	{
		phoneBGR24Queue.swap(out);
	}
	else if (2 == type)
	{
		sleepBGR24Queue.swap(out);
	}
	else if(3 == type)
	{
		fightBGR24Queue.swap(out);
	}
	else if (4 == type)
	{
		faceBGR24Queue.swap(out);
	}
}

void DigitCameraLivestream::captureVideoDataNotifiy(
	const unsigned char* streamData /* = NULL */, const long dataBytes /* = 0 */, const long dataType /* = -1 */)
{
	if (stopped)
	{
		condition[0].notify_one();
		return;
	}

	if (!algorithmAbility)
	{
		return;
	}

	H264Frame* h264Frame{ new(std::nothrow) H264Frame };
	if (h264Frame)
	{
		h264Frame->frame = new(std::nothrow) unsigned char[dataBytes];
		if (h264Frame->frame)
		{
			memcpy_s(h264Frame->frame, dataBytes, streamData, dataBytes);
			h264Frame->bytes = dataBytes;

			if (!h264LivestreamQueue.insert(h264Frame))
			{
				boost::checked_array_delete(h264Frame->frame);
				boost::checked_delete(h264Frame);
			}
		}
		else
		{
			boost::checked_delete(h264Frame);
		}
	}
}

void DigitCameraLivestream::JPEGPFrameEncodeHandler(const char* data /* = NULL */, const int dataBytes /* = 0 */)
{}

// void DigitCameraChannel::audioDataCaptureNotifier(const char* data /* = NULL */, const int dataBytes /* = 0 */)
// {}

void DigitCameraLivestream::videoStreamDecodeHandler(
	const char* frameData /* = NULL */, const int dataBytes /* = 0 */, const unsigned long long frameNumber /* = 0 */, 
	const NS(decoder, 1)::DecodeFrame& decodeFrame /* = NS(decoder, 1)::DecodeFrame::DECODE_FRAME_NONE */, 
	const int width /* = 0 */, const int height /* = 0 */)
{
	if (stopped)
	{
		condition[1].notify_one();
		return;
	}

	//If not sailing, do nothing.
	if (0 < sailingStatus || (1 != (++livestreamFrameNumber % 3)))
	{
		return;
	}

	if (videoFrameScalerPtr && jpegFrameEncoderPtr)
	{
		char* jpegFrameData{ NULL };
		int jpegFrameDataBytes{ 0 };
		jpegFrameEncoderPtr->encode(jpegFrameData, jpegFrameDataBytes, frameData, dataBytes, width, height);
		const int bgr24FrameDataBytes{ width * height * 3 };
		const char* bgr24FrameData{ videoFrameScalerPtr->scale(frameData, dataBytes, width, height) };

// 		if (sdlTexture && sdlRenderer)
// 		{
// 			c24To32((unsigned char*)bgr24FrameData, covertBuffer, width, height);
// 			SDL_UpdateTexture(sdlTexture, NULL, covertBuffer, width * 4);
// 			SDL_Rect rc;
// 			rc.x = 0;
// 			rc.y = 0;
// 			rc.w = 960;
// 			rc.h = 540;
// 			SDL_RenderClear(sdlRenderer);
// 			SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, &rc);
// 			SDL_RenderPresent(sdlRenderer);
// 		}

		if (bgr24FrameData)
		{
			BGR24Frame* bgr24Frame = new(std::nothrow) BGR24Frame;
			bool bgr24FrameNew{ false };

			if (bgr24Frame)
			{
				bgr24Frame->frameData = new(std::nothrow) char[bgr24FrameDataBytes];
				bgr24Frame->jpegData = new(std::nothrow) char[jpegFrameDataBytes];
				const int iplen{ (const int)NVRIp.length() };
				bgr24Frame->NVRIp = new(std::nothrow) char[iplen + 1];

				if (bgr24Frame->frameData && bgr24Frame->jpegData && bgr24Frame->NVRIp)
				{
					bgr24Frame->frameBytes = bgr24FrameDataBytes;
					memcpy_s(bgr24Frame->frameData, bgr24FrameDataBytes, bgr24FrameData, bgr24FrameDataBytes);
					memcpy_s(bgr24Frame->jpegData, jpegFrameDataBytes, jpegFrameData, jpegFrameDataBytes);
					bgr24Frame->channelIndex = cameraIndex;
					bgr24Frame->jpegBytes = jpegFrameDataBytes;
					bgr24Frame->NVRIp[iplen] = 0;
					memcpy_s(bgr24Frame->NVRIp, iplen, NVRIp.c_str(), iplen);
				}
				else
				{
					deleteBGR24Frame(bgr24Frame);
					return;
				}

				if (algorithmAbility & 0x01)
				{
					BGR24Frame* frame{ newBGR24Frame(bgr24Frame) };
					if (frame)
					{
						bgr24FrameNew = helmetBGR24Queue.insert(frame);
						if (!bgr24FrameNew)
						{
							deleteBGR24Frame(frame);
						}
					}
				}
				if ((algorithmAbility >> 1) & 0x01)
				{
					BGR24Frame* frame{ newBGR24Frame(bgr24Frame) };
					if (frame)
					{
						bgr24FrameNew = phoneBGR24Queue.insert(frame);
						if (!bgr24FrameNew)
						{
							deleteBGR24Frame(frame);
						}
					}
				}
				if ((algorithmAbility >> 2) & 0x01)
				{
					BGR24Frame* frame{ newBGR24Frame(bgr24Frame) };
					if (frame)
					{
						bgr24FrameNew = sleepBGR24Queue.insert(frame);
						if (!bgr24FrameNew)
						{
							deleteBGR24Frame(frame);
						}
					}
				}
				if ((algorithmAbility >> 3) & 0x01)
				{
					BGR24Frame* frame{ newBGR24Frame(bgr24Frame) };
					if (frame)
					{
						bgr24FrameNew = fightBGR24Queue.insert(frame);
						if (!bgr24FrameNew)
						{
							deleteBGR24Frame(frame);
						}
					}
				}
				if ((algorithmAbility >> 4) & 0x01)
				{
					BGR24Frame* frame{ newBGR24Frame(bgr24Frame) };
					if (frame)
					{
						bgr24FrameNew = faceBGR24Queue.insert(frame);
						if (!bgr24FrameNew)
						{
							deleteBGR24Frame(frame);
						}
					}
				}

				deleteBGR24Frame(bgr24Frame);
			}
		}

		boost::checked_array_delete(jpegFrameData);
 	}
}

DWORD DigitCameraLivestream::frameDecodeProcessThread(void* ctx /* = NULL */)
{
	DigitCameraLivestream* livestream{ reinterpret_cast<DigitCameraLivestream*>(ctx) };

	while (livestream)
	{
		if (livestream->stopped)
		{
			livestream->condition[2].notify_one();
			return 0;
		}

		H264Frame* h264Frame{ reinterpret_cast<H264Frame*>(livestream->h264LivestreamQueue.remove()) };
		if (h264Frame)
		{
			livestream->videoStreamDecoderPtr->decode(h264Frame->frame, h264Frame->bytes, 0);
			boost::checked_array_delete(h264Frame->frame);
			boost::checked_delete(h264Frame);
		}
	}

	return 0;
}

// int DigitCameraLivestream::refreshSDLVideo(void* ctx /* = NULL */)
// {
// 	while (1)
// 	{
// 		SDL_Event e;
// 		e.type = SDL_DISPLAYEVENT;
// 		SDL_PushEvent(&e);
// 		SDL_Delay(1);
// 	}
// }

BGR24Frame* DigitCameraLivestream::newBGR24Frame(const BGR24Frame* frame /*= NULL*/)
{
	BGR24Frame* bgr24FrameCopyData = NULL;

	if (frame)
	{
		bgr24FrameCopyData = new(std::nothrow) BGR24Frame;
		if (bgr24FrameCopyData)
		{
			bgr24FrameCopyData->frameData = new(std::nothrow) char[frame->frameBytes];
			bgr24FrameCopyData->jpegData = new(std::nothrow) char[frame->jpegBytes];
			const int iplen{ (int)strlen(frame->NVRIp) };
			bgr24FrameCopyData->NVRIp = new(std::nothrow) char[iplen + 1];

			if (bgr24FrameCopyData->frameData && bgr24FrameCopyData->jpegData && bgr24FrameCopyData->NVRIp)
			{
				bgr24FrameCopyData->frameBytes = frame->frameBytes;
				memcpy_s(bgr24FrameCopyData->frameData, frame->frameBytes, frame->frameData, frame->frameBytes);
				memcpy_s(bgr24FrameCopyData->jpegData, frame->jpegBytes, frame->jpegData, frame->jpegBytes);
				bgr24FrameCopyData->channelIndex = frame->channelIndex;
				bgr24FrameCopyData->jpegBytes = frame->jpegBytes;
				bgr24FrameCopyData->NVRIp[iplen] = 0;
				memcpy_s(bgr24FrameCopyData->NVRIp, iplen, frame->NVRIp, iplen);
			}
			else
			{
				deleteBGR24Frame(bgr24FrameCopyData);
			}
		}
	}

	return bgr24FrameCopyData;
}

void DigitCameraLivestream::deleteBGR24Frame(BGR24Frame* frame)
{
	if (frame)
	{
		boost::checked_array_delete(frame->frameData);
		boost::checked_array_delete(frame->jpegData);
		boost::checked_array_delete(frame->NVRIp);
		boost::checked_delete(frame);
	}
}
