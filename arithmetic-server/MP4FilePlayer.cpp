#include <iostream>
#include <fstream>
#include <io.h>
#include "boost/make_shared.hpp"
#include "error.h"
#include "AlarmMessage.h"
#include "MP4FilePlayer.h"

MP4FilePlayer::MP4FilePlayer(
	PublisherServerModel& publisher, 
	CHelmetAlgorithmClass& helmet, 
	CPhoneAlgorithmClass& phone, 
	CSleepAlgorithmClass& sleep, 
	const char* filePath /* = NULL */)
	: MediaFileDecoder(filePath), publisherServerModel{ publisher },
	helmetDetectAlgo{ helmet }, phoneDectectAlgo{ phone }, sleepDectectAlgo{ sleep }
{}

MP4FilePlayer::~MP4FilePlayer()
{}

int MP4FilePlayer::initializeDecoder()
{
	int status{ ERR_BAD_ALLOC };

	boost::shared_ptr<FrameScaler> scaler{ boost::make_shared<FFmpegFrameScaler>(AV_PIX_FMT_YUV420P, AV_PIX_FMT_YUVJ422P) };
	boost::shared_ptr<MediaRenderer> renderer{ boost::make_shared<SDLRenderer>() };
	if (renderer)
	{
		sdlVideoRendererPtr.swap(renderer);
		sdlVideoRendererPtr->createRenderer(true);

		jpegFrameScalerPtr.swap(scaler);
		jpegFrameScalerPtr->initialize();
		status = ERR_OK;
	}

	return ERR_OK == status ? MediaFileDecoder::initializeDecoder() : status;
}

void MP4FilePlayer::uninitializeDecoder()
{
	if (sdlVideoRendererPtr)
	{
		sdlVideoRendererPtr->destroyRenderer();
		sdlVideoRendererPtr.reset();
	}
	
	if (jpegFrameScalerPtr)
	{
		jpegFrameScalerPtr->uninitialize();
		jpegFrameScalerPtr.reset();
	}
}

void MP4FilePlayer::gotMediaFrameDecode(const char* data /* = NULL */, const AVFrame* yuv420p /* = NULL */)
{
	if (sdlVideoRendererPtr)
	{
		NS(scaler, 1)::EncodeYUVToJPEG((AVFrame*)yuv420p, NULL, 1920, 1080);
		processAlgoResult(data);
		sdlVideoRendererPtr->pushData(data);
	}
}

void MP4FilePlayer::processAlgoResult(
	const char* data /* = NULL */, const int pixelWidth /* = 0 */, const int pixelHeight /* = 0 */)
{
// 	FeedBackHelmet helmetFeedback{};
// 	if (helmetDetectAlgo.MainProcFunc((unsigned char*)data, helmetFeedback))
// 	{
// 		for (int i = 0; i != helmetFeedback.vecShowInfo.size(); ++i)
// 		{
// 			// 			boost::shared_ptr<NS(scaler, 1)::FrameScaler> frameScalerPtr{
// 			// 				boost::make_shared<NS(scaler, 1)::FFmpegFrameScaler>(AV_PIX_FMT_BGR24, AV_PIX_FMT_J)};
// // 			std::cout << "Helmet Target " << helmetFeedback.vecShowInfo[i].nLabel
// // 				<< " found x = " << helmetFeedback.vecShowInfo[i].rRect.x
// // 				<< " y = " << helmetFeedback.vecShowInfo[i].rRect.y
// // 				<< " w = " << helmetFeedback.vecShowInfo[i].rRect.width
// // 				<< " h = " << helmetFeedback.vecShowInfo[i].rRect.height << std::endl;
// 
// 			if (jpegFrameScalerPtr)
// 			{
// //				const char* jpegData{ jpegFrameScalerPtr->inputRaw(data, pixelHeight * pixelWidth * 3, pixelWidth, pixelHeight) };
// 				if (1/*jpegData*/)
// 				{
// //					std::vector<RECT> rects;
// 					AlarmMessage am;
// 					AlarmRect rect;
// 					rect.x = helmetFeedback.vecShowInfo[i].rRect.x;
// 					rect.y = helmetFeedback.vecShowInfo[i].rRect.y;
// 					rect.w = helmetFeedback.vecShowInfo[i].rRect.width;
// 					rect.h = helmetFeedback.vecShowInfo[i].rRect.height;
// //					rects.push_back(rect);
// 
// 					FILE* f{ NULL };
// 					fopen_s(&f, "./Sample/test.jpg", "rb+");
// 					const int jpegBytes{ _filelength(_fileno(f)) };
// 					char* jpegData = new(std::nothrow) char[jpegBytes];
// 					fread(jpegData, jpegBytes, 1, f);
// 					fclose(f);
// 
// 					am.setMessageData(0, pixelWidth, pixelHeight, 0, "HelmetAlarm", helmetFeedback.vecShowInfo[i].nLabel, rect, jpegData, jpegBytes);
// 					publisherServerModel.send(am.getMessageData(), am.getMessageBytes());
// 				}
// 			}
// 		}
// 
// 		helmetDetectAlgo.PostProcessFunc(helmetFeedback);
// 	}
// 
// 	FeedBackPhone phoneFeedback{};
// 	if (phoneDectectAlgo.MainProcFunc((unsigned char*)data, phoneFeedback))
// 	{
// 		for (int i = 0; i != phoneFeedback.vecShowInfo.size(); ++i)
// 		{
// // 			LOG(INFO) << "Phone Target " << phoneFeedback.vecShowInfo[i].nSerioNo
// // 				<< " found x = " << phoneFeedback.vecShowInfo[i].rRect.x
// // 				<< " y = " << phoneFeedback.vecShowInfo[i].rRect.y
// // 				<< " w = " << phoneFeedback.vecShowInfo[i].rRect.width
// // 				<< " h = " << phoneFeedback.vecShowInfo[i].rRect.height;
// 
// 			AlarmMessage am;
// 			AlarmRect rect;
// 			rect.x = helmetFeedback.vecShowInfo[i].rRect.x;
// 			rect.y = helmetFeedback.vecShowInfo[i].rRect.y;
// 			rect.w = helmetFeedback.vecShowInfo[i].rRect.width;
// 			rect.h = helmetFeedback.vecShowInfo[i].rRect.height;
// 			//					rects.push_back(rect);
// 
// 			FILE* f{ NULL };
// 			fopen_s(&f, "./Sample/test.jpg", "rb+");
// 			const int jpegBytes{ _filelength(_fileno(f)) };
// 			char* jpegData = new(std::nothrow) char[jpegBytes];
// 			fread(jpegData, jpegBytes, 1, f);
// 			fclose(f);
// 
// 			am.setMessageData(0, pixelWidth, pixelHeight, 1, "PhoneAlarm", helmetFeedback.vecShowInfo[i].nLabel, rect, jpegData, jpegBytes);
// 			publisherServerModel.send(am.getMessageData(), am.getMessageBytes());
// 		}
// 
// 		phoneDectectAlgo.PostProcessFunc(phoneFeedback);
// 	}
// 
// 	FeedBackSleep sleepFeedback{};
// 	if (sleepDectectAlgo.MainProcFunc((unsigned char*)data, sleepFeedback))
// 	{
// 		for (int i = 0; i != sleepFeedback.vecShowInfo.size(); ++i)
// 		{
// // 			LOG(INFO) << "Sleep Target " << sleepFeedback.vecShowInfo[i].nSerioNo
// // 				<< " found x = " << sleepFeedback.vecShowInfo[i].rRect.x
// // 				<< " y = " << sleepFeedback.vecShowInfo[i].rRect.y
// // 				<< " w = " << sleepFeedback.vecShowInfo[i].rRect.width
// // 				<< " h = " << sleepFeedback.vecShowInfo[i].rRect.height;
// 
// 			AlarmMessage am;
// 			AlarmRect rect;
// 			rect.x = helmetFeedback.vecShowInfo[i].rRect.x;
// 			rect.y = helmetFeedback.vecShowInfo[i].rRect.y;
// 			rect.w = helmetFeedback.vecShowInfo[i].rRect.width;
// 			rect.h = helmetFeedback.vecShowInfo[i].rRect.height;
// 			//					rects.push_back(rect);
// 
// 			FILE* f{ NULL };
// 			fopen_s(&f, "./Sample/test.jpg", "rb+");
// 			const int jpegBytes{ _filelength(_fileno(f)) };
// 			char* jpegData = new(std::nothrow) char[jpegBytes];
// 			fread(jpegData, jpegBytes, 1, f);
// 			fclose(f);
// 
// 			am.setMessageData(0, pixelWidth, pixelHeight, 2, "SleepAlarm", helmetFeedback.vecShowInfo[i].nLabel, rect, jpegData, jpegBytes);
// 			publisherServerModel.send(am.getMessageData(), am.getMessageBytes());
// 		}
// 
// 		sleepDectectAlgo.PostProcessFunc(sleepFeedback);
// 	}
}
