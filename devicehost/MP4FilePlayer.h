#ifndef MP4_FILE_PLAYER_H
#define MP4_FILE_PLAYER_H

#include "boost/shared_ptr.hpp"
#include "MediaDecoder/File/MediaFileDecoder.h"
using MediaFileDecoder = NS(decoder, 1)::MediaFileDecoder;
#include "MediaRenderer/SDLRenderer.h"
using MediaRenderer = NS(renderer, 1)::MediaRenderer;
using SDLRenderer = NS(renderer, 1)::SDLRenderer;
#include "MQModel/PublisherServerModel.h"
using PublisherServerModel = NS(model, 1)::PublisherServerModel;
#include "MediaScaler/FFmpeg/FFmpegFrameScaler.h"
using FrameScaler = NS(scaler, 1)::FrameScaler;
using FFmpegFrameScaler = NS(scaler, 1)::FFmpegFrameScaler;
#include "BaseHelmetDll.h"
#include "BasePhoneDll.h"
#include "BaseSleepDll.h"

class MP4FilePlayer final : public MediaFileDecoder
{
public:
	MP4FilePlayer(
		PublisherServerModel& publisher, 
		CHelmetAlgorithmClass& helmet,
		CPhoneAlgorithmClass& phone,
		CSleepAlgorithmClass& sleep,
		const char* filePath = NULL);
	~MP4FilePlayer(void);

protected:
	int initializeDecoder(void) override;
	void uninitializeDecoder(void) override;
	void gotMediaFrameDecode(const char* data = NULL, const AVFrame* yuv420p = NULL) override;

private:
	void processAlgoResult(const char* data = NULL, const int pixelWidth = 1920, const int pixelHeight = 1080);

private:
	PublisherServerModel& publisherServerModel;
	boost::shared_ptr<MediaRenderer> sdlVideoRendererPtr;
	boost::shared_ptr<FrameScaler> jpegFrameScalerPtr;

	CHelmetAlgorithmClass& helmetDetectAlgo;
	CPhoneAlgorithmClass& phoneDectectAlgo;
	CSleepAlgorithmClass& sleepDectectAlgo;
};//class MP4FilePlayer

#endif//MP4_FILE_PLAYER_H

