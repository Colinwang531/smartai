#include "boost/bind.hpp"
#include "boost/filesystem/path.hpp"
#include "boost/filesystem/operations.hpp"
#include "boost/make_shared.hpp"
#ifdef _WINDOWS
#include "glog/log_severity.h"
#include "glog/logging.h"
#else
#include "glog/log_severity.h"
#include "glog/logging.h"
#endif//_WINDOWS
#include "error.h"
#include "MediaDecoder/SDK/HikvisionSDKDecoder.h"
using HikvisionSDKDecoder = NS(decoder, 1)::HikvisionSDKDecoder;
#include "MediaConverter/YV12ToYUV420PConverter.h"
using YV12ToYUV420PConverter = NS(converter, 1)::YV12ToYUV420PConverter;
#include "MediaConverter/YUV420PToBGR24Converter.h"
using YUV420PToBGR24Converter = NS(converter, 1)::YUV420PToBGR24Converter;
#include "MediaEncoder/JPEG/YUV420PToJPEGEncoder.h"
using YUV420PToJPEGEncoder = NS(encoder, 1)::YUV420PToJPEGEncoder;
#include "Arithmetic/CVAlgoHelmet.h"
using CVAlgoHelmet = NS(algo, 1)::CVAlgoHelmet;
#include "Arithmetic/CVAlgoPhone.h"
using CVAlgoPhone = NS(algo, 1)::CVAlgoPhone;
#include "Arithmetic/CVAlgoSleep.h"
using CVAlgoSleep = NS(algo, 1)::CVAlgoSleep;
#include "Arithmetic/CVAlgoFight.h"
using CVAlgoFight = NS(algo, 1)::CVAlgoFight;
#include "Arithmetic/CVAlgoFace.h"
using CVAlgoFace = NS(algo, 1)::CVAlgoFace;
#include "AlarmMessage.h"
#include "DigitCameraLivestream.h"

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

DigitCameraLivestream::DigitCameraLivestream(
	const long uid /* = -1 */, const unsigned short idx /* = -1 */)
	: HikvisionLivestream(uid, idx), arithmeticAbilities{ 0 }, stopped{ false }, livestreamFrameNumber{ 0 }//,
//	sdlWindow{ NULL }, sdlRenderer{ NULL }, sdlTexture{ NULL }
{}

DigitCameraLivestream::~DigitCameraLivestream()
{}

int DigitCameraLivestream::openStream()
{
	int status{ ERR_BAD_ALLOC };
	boost::shared_ptr<MediaDecoder> videoDecoderPtr{ 
		boost::make_shared<HikvisionSDKDecoder>(
			boost::bind(&DigitCameraLivestream::videoStreamDecodeHandler, this, _1, _2, _3, _4, _5, _6)) };
	boost::shared_ptr<MediaConverter> yv12ToYuv420pConverterPtr{ boost::make_shared<YV12ToYUV420PConverter>() };
	boost::shared_ptr<MediaConverter> yuv420pToConverterPtr{ boost::make_shared<YV12ToYUV420PConverter>() };
	boost::shared_ptr<MediaEncoder> jpegEncoderPtr{ boost::make_shared<YUV420PToJPEGEncoder>() };

	if (videoDecoderPtr && yv12ToYuv420pConverterPtr && yuv420pToConverterPtr && jpegEncoderPtr)
	{
		videoStreamDecoderPtr.swap(videoDecoderPtr);
 		yv12FrameConverterPtr.swap(yv12ToYuv420pConverterPtr);
		yuv420pFrameConverterPtr.swap(yuv420pToConverterPtr);
		jpegPictureEncoderPtr.swap(jpegEncoderPtr);

		h264LivestreamQueue.setCapacity(1000);
		DWORD threadID{ 0 };
		HANDLE handle{ 
			CreateThread(NULL, 0, &DigitCameraLivestream::frameDecodeProcessThread, this, 0, &threadID) };
		SetThreadPriority(handle, THREAD_PRIORITY_HIGHEST);

		status = HikvisionLivestream::openStream() ? ERR_OK : ERR_BAD_OPERATE;

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

int DigitCameraLivestream::closeStream()
{
	int status{ HikvisionLivestream::closeStream() };

	if (ERR_OK == status)
	{
		stopped = true;
		boost::unique_lock<boost::mutex> lock1{ mtx[0] }, lock2{ mtx[1] }, lock3{ mtx[2] };
		condition[0].wait_for(lock1, boost::chrono::seconds(1));
		condition[1].wait_for(lock2, boost::chrono::seconds(1));
		condition[2].wait_for(lock3, boost::chrono::seconds(1));

		helmetArithmeticPtr.reset();
		phoneArithmeticPtr.reset();
		sleepArithmeticPtr.reset();
		fightArithmeticPtr.reset();
		faceArithmeticPtr.reset();
	}

	return status;
}


void DigitCameraLivestream::setArithmeticAbilities(const unsigned int abilities /* = 0 */)
{
	arithmeticAbilities = abilities;
	const std::string exePath{
		boost::filesystem::initial_path<boost::filesystem::path>().string() };

	if (arithmeticAbilities & 0x01)
	{
		if (!helmetArithmeticPtr)
		{
			CVAlgoPtr helmet{ 
				boost::make_shared<CVAlgoHelmet>(
					boost::bind(&DigitCameraLivestream::alarmInfoProcessHandler, this, _1, _2)) };
			if (helmet)
			{
				if (helmet->initialize(exePath.c_str(), /*0.25f*/0.95f, 0.10f))
				{
					helmetArithmeticPtr.swap(helmet);
					LOG(INFO) << "Initialize HELMET arithmetic Successfully.";
				}
				else
				{
					LOG(WARNING) << "Initialize HELMET arithmetic Failed.";
				}
			}
		}
	}
	else
	{
		if (helmetArithmeticPtr)
		{
			helmetArithmeticPtr.reset();
		}
	}

	if ((arithmeticAbilities >> 1) & 0x01)
	{
		if (!phoneArithmeticPtr)
		{
			CVAlgoPtr phone{ 
				boost::make_shared<CVAlgoPhone>(
					boost::bind(&DigitCameraLivestream::alarmInfoProcessHandler, this, _1, _2)) };
			if (phone)
			{
				if (phone->initialize(exePath.c_str(), /*0.25f*/0.95f, 0.10f))
				{
					phoneArithmeticPtr.swap(phone);
					LOG(INFO) << "Initialize PHONE arithmetic Successfully.";
				}
				else
				{
					LOG(WARNING) << "Initialize PHONE arithmetic Failed.";
				}
			}
		}
	}
	else
	{
		if (phoneArithmeticPtr)
		{
			phoneArithmeticPtr.reset();
		}
	}

	if ((arithmeticAbilities >> 2) & 0x01)
	{
		if (!sleepArithmeticPtr)
		{
			CVAlgoPtr sleep{ 
				boost::make_shared<CVAlgoSleep>(
					boost::bind(&DigitCameraLivestream::alarmInfoProcessHandler, this, _1, _2)) };
			if (sleep)
			{
				if (sleep->initialize(exePath.c_str(), /*0.25f*/0.95f, 0.10f))
				{
					sleepArithmeticPtr.swap(sleep);
					LOG(INFO) << "Initialize SLEEP arithmetic Successfully.";
				}
				else
				{
					LOG(WARNING) << "Initialize SLEEP arithmetic Failed.";
				}
			}
		}
	}
	else
	{
		if (sleepArithmeticPtr)
		{
			sleepArithmeticPtr.reset();
		}
	}

	if ((arithmeticAbilities >> 3) & 0x01)
	{
		if (!fightArithmeticPtr)
		{
			CVAlgoPtr fight{ 
				boost::make_shared<CVAlgoFight>(
					boost::bind(&DigitCameraLivestream::alarmInfoProcessHandler, this, _1, _2)) };
			if (fight)
			{
				if (fight->initialize(exePath.c_str(), /*0.25f*/0.95f, 0.10f))
				{
					fightArithmeticPtr.swap(fight);
					LOG(INFO) << "Initialize FIGHT arithmetic Successfully.";
				}
				else
				{
					LOG(WARNING) << "Initialize FIGHT arithmetic Failed.";
				}
			}
		}
	}
	else
	{
		if (fightArithmeticPtr)
		{
			fightArithmeticPtr.reset();
		}
	}

	if ((arithmeticAbilities >> 4) & 0x01)
	{
		if (!faceArithmeticPtr)
		{
			CVAlgoPtr face{ 
				boost::make_shared<CVAlgoFace>(
					boost::bind(&DigitCameraLivestream::alarmInfoProcessHandler, this, _1, _2)) };
			if (face)
			{
				if (face->initialize(exePath.c_str(), /*0.25f*/0.95f, 0.10f))
				{
					faceArithmeticPtr.swap(face);
					LOG(INFO) << "Initialize FACE arithmetic Successfully.";
				}
				else
				{
					LOG(WARNING) << "Initialize FACE arithmetic Failed.";
				}
			}
		}
	}
	else
	{
		if (faceArithmeticPtr)
		{
			faceArithmeticPtr.reset();
		}
	}
}

// void DigitCameraLivestream::queue(const int type, std::vector<void*>& out)
// {
// 	if (0 == type)
// 	{
// 		helmetBGR24Queue.swap(out);
// 	} 
// 	else if (1 == type)
// 	{
// 		phoneBGR24Queue.swap(out);
// 	}
// 	else if (2 == type)
// 	{
// 		sleepBGR24Queue.swap(out);
// 	}
// 	else if(3 == type)
// 	{
// 		fightBGR24Queue.swap(out);
// 	}
// 	else if (4 == type)
// 	{
// 		faceBGR24Queue.swap(out);
// 	}
// }

void DigitCameraLivestream::captureVideoStreamProcess(
	const unsigned char* data /* = NULL */, const long long dataBytes /* = 0 */)
{
	if (stopped)
	{
		condition[0].notify_one();
		return;
	}

	if (!arithmeticAbilities)
	{
		return;
	}

	MediaImagePtr h264ImagePtr{ 
		boost::make_shared<MediaImage>(NS(frame, 1)::MediaImageType::MEDIA_IMAGE_H264) };
	if (h264ImagePtr)
	{
		if (ERR_OK == h264ImagePtr->setImage(data, dataBytes))
		{
			h264LivestreamQueue.insert(h264ImagePtr);
		}
	}
}

void DigitCameraLivestream::JPEGPFrameEncodeHandler(const char* data /* = NULL */, const int dataBytes /* = 0 */)
{}

// void DigitCameraChannel::audioDataCaptureNotifier(const char* data /* = NULL */, const int dataBytes /* = 0 */)
// {}

void DigitCameraLivestream::videoStreamDecodeHandler(
	const char* frameData /* = NULL */, const long frameBytes /* = 0 */, 
	const long imageWidth /* = 0 */, const long imageHeight /* = 0 */)
{
	if (stopped)
	{
		condition[1].notify_one();
		return;
	}

	//If not sailing, do nothing.
	if (1 == sailingStatus/* || (1 != (++livestreamFrameNumber % 3))*/)
	{
		return;
	}

	if (yv12FrameConverterPtr && yuv420pFrameConverterPtr)
	{
		const long imageDataBytes{ imageWidth * imageHeight * 3 };
		const unsigned char* yuv420ImageData{ 
			yv12FrameConverterPtr->convert((const unsigned char*)frameData, frameBytes, imageWidth, imageHeight) };
		const unsigned char* bgr24ImageData{
			yuv420pFrameConverterPtr->convert(yuv420ImageData, imageDataBytes, imageWidth, imageHeight) };

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

		if (bgr24ImageData)
		{
			MediaImagePtr bgr24ImagePtr{
				boost::make_shared<NS(frame, 1)::MediaImage>(NS(frame, 1)::MediaImageType::MEDIA_IMAGE_BGR24) };

			if (bgr24ImagePtr)
			{
				bgr24ImagePtr->setOriginImage((const unsigned char*)frameData, frameBytes);
				bgr24ImagePtr->setImage(bgr24ImageData, imageDataBytes);

				if (helmetArithmeticPtr)
				{
					helmetArithmeticPtr->tryInputMediaImage(bgr24ImagePtr);
				}
				if (phoneArithmeticPtr)
				{
					phoneArithmeticPtr->tryInputMediaImage(bgr24ImagePtr);
				}
				if (sleepArithmeticPtr)
				{
					sleepArithmeticPtr->tryInputMediaImage(bgr24ImagePtr);
				}
				if (fightArithmeticPtr)
				{
					fightArithmeticPtr->tryInputMediaImage(bgr24ImagePtr);
				}
				if (faceArithmeticPtr)
				{
					faceArithmeticPtr->tryInputMediaImage(bgr24ImagePtr);
				}
			}
		}

//		boost::checked_array_delete(jpegFrameData);
 	}
}

DWORD DigitCameraLivestream::frameDecodeProcessThread(void* ctx /* = NULL */)
{
	DigitCameraLivestream* livestream{ reinterpret_cast<DigitCameraLivestream*>(ctx) };

	while (livestream && livestream->videoStreamDecoderPtr)
	{
		if (livestream->stopped)
		{
			livestream->condition[2].notify_one();
			return 0;
		}

		MediaImagePtr h264ImagePtr{ livestream->h264LivestreamQueue.remove() };
		if (h264ImagePtr)
		{
			livestream->videoStreamDecoderPtr->decode(h264ImagePtr->getImage(), h264ImagePtr->getImageBytes());
		}
	}

	return 0;
}

void DigitCameraLivestream::alarmInfoProcessHandler(
	MediaImagePtr image, std::vector<NS(algo, 1)::AlarmInfo> alarmInfos)
{
	if (image && 0 < alarmInfos.size())
	{
		AlarmMessage message;
		message.setMessageData(
			(int)alarmInfos[0].type, 1920, 1080, bgr24Frame->NVRIp, streamIndex, alarmInfos, bgr24Frame->jpegData, bgr24Frame->jpegBytes);
		boost::shared_ptr<PublisherModel> publisherModel{
			boost::dynamic_pointer_cast<PublisherModel>(publisherModelPtr) };

		publishMtx.lock();
		publisherModel->send(message.getMessageData(), message.getMessageBytes());
		publishMtx.unlock();

		LOG(INFO) << "Send push alarm " << bgr24Frame->NVRIp << "_" << bgr24Frame->channelIndex << "_" << detectInfos[0].type;
	}
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

// BGR24Frame* DigitCameraLivestream::newBGR24Frame(const BGR24Frame* frame /*= NULL*/)
// {
// 	BGR24Frame* bgr24FrameCopyData = NULL;
// 
// 	if (frame)
// 	{
// 		bgr24FrameCopyData = new(std::nothrow) BGR24Frame;
// 		if (bgr24FrameCopyData)
// 		{
// 			bgr24FrameCopyData->frameData = new(std::nothrow) char[frame->frameBytes];
// 			bgr24FrameCopyData->jpegData = new(std::nothrow) char[frame->jpegBytes];
// 			const int iplen{ (int)strlen(frame->NVRIp) };
// 			bgr24FrameCopyData->NVRIp = new(std::nothrow) char[iplen + 1];
// 
// 			if (bgr24FrameCopyData->frameData && bgr24FrameCopyData->jpegData && bgr24FrameCopyData->NVRIp)
// 			{
// 				bgr24FrameCopyData->frameBytes = frame->frameBytes;
// 				memcpy_s(bgr24FrameCopyData->frameData, frame->frameBytes, frame->frameData, frame->frameBytes);
// 				memcpy_s(bgr24FrameCopyData->jpegData, frame->jpegBytes, frame->jpegData, frame->jpegBytes);
// 				bgr24FrameCopyData->channelIndex = frame->channelIndex;
// 				bgr24FrameCopyData->jpegBytes = frame->jpegBytes;
// 				bgr24FrameCopyData->NVRIp[iplen] = 0;
// 				memcpy_s(bgr24FrameCopyData->NVRIp, iplen, frame->NVRIp, iplen);
// 			}
// 			else
// 			{
// 				deleteBGR24Frame(bgr24FrameCopyData);
// 			}
// 		}
// 	}
// 
// 	return bgr24FrameCopyData;
// }
// 
// void DigitCameraLivestream::deleteBGR24Frame(BGR24Frame* frame)
// {
// 	if (frame)
// 	{
// 		boost::checked_array_delete(frame->frameData);
// 		boost::checked_array_delete(frame->jpegData);
// 		boost::checked_array_delete(frame->NVRIp);
// 		boost::checked_delete(frame);
// 	}
// }
