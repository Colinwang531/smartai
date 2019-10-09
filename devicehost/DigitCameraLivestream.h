#ifndef DIGITAL_CAMERA_LIVE_STREAM_H
#define DIGITAL_CAMERA_LIVE_STREAM_H

#include "boost/shared_ptr.hpp"
#include "boost/thread/condition.hpp"
#include "MediaFrame/MediaFrame.h"
using MediaImage = NS(frame, 1)::MediaImage;
#include "Stream/Hikvision/HikvisionLivestream.h"
using HikvisionLivestream = NS(stream, 1)::HikvisionLivestream;
#include "MediaDecoder/MediaDecoder.h"
using MediaDecoder = NS(decoder, 1)::MediaDecoder;
#include "MediaConverter/MediaConverter.h"
using MediaConverter = NS(converter, 1)::MediaConverter;
#include "MediaEncoder/JPEG/YUV420PToJPEGEncoder.h"
using MediaEncoder = NS(encoder, 1)::MediaEncoder;
#include "Arithmetic/CVAlgo.h"
using CVAlgoPtr = boost::shared_ptr<NS(algo, 1)::CVAlgo>;
#include "AsynchronousServer.h"
using MQModel = NS(model, 1)::MQModel;

class DigitCameraLivestream final : public HikvisionLivestream
{
public:
	DigitCameraLivestream(
		boost::shared_ptr<MQModel> publisher, const std::string NVRIp, 
		const long uid = -1, const unsigned short idx = -1);
	~DigitCameraLivestream(void);

public:
	int openStream(void) override;
	int closeStream(void) override;
	void setArithmeticAbilities(const unsigned int abilities = 0);
	int addFacePicture(const char* filePath = NULL, const int faceID = 0);

protected:
	unsigned long long captureJPEGPicture(
		const char* data = NULL, const unsigned long long dataBytes = 0) override;
	void captureVideoStreamProcess(
		const unsigned char* data = NULL, const long long dataBytes = 0) override;

private:
	void videoStreamDecodeHandler(
		const char* frameData = NULL, const long frameBytes = 0, 
		const long imageWidth = 0, const long imageHeight = 0);
	void JPEGPFrameEncodeHandler(const char* data = NULL , const int dataBytes = 0 );
	static DWORD WINAPI frameDecodeProcessThread(void* ctx = NULL);
	void alarmInfoProcessHandler(
		MediaImagePtr image, std::vector<NS(algo, 1)::AlarmInfo> alarmInfos);

private:
	boost::shared_ptr<MediaDecoder> videoStreamDecoderPtr;
	boost::shared_ptr<MediaConverter> yv12ToYuv420pConverterPtr;
	boost::shared_ptr<MediaConverter> yuv420pToBGR24ConverterPtr;
	boost::shared_ptr<MediaEncoder> jpegPictureEncoderPtr;
	CVAlgoPtr helmetArithmeticPtr;
	CVAlgoPtr phoneArithmeticPtr;
	CVAlgoPtr sleepArithmeticPtr;
	CVAlgoPtr fightArithmeticPtr;
	CVAlgoPtr faceArithmeticPtr;
	FIFOQueue h264LivestreamQueue;

	unsigned long long livestreamFrameNumber;
	unsigned int arithmeticAbilities;
	const std::string NVRIpAddress;
	boost::mutex publisherMtx;
	boost::shared_ptr<MQModel> publisherModelPtr;
	bool stopped;
	//Guarantee work thread exited safely. 
	boost::mutex mtx[3];
	boost::condition condition[3];
};//class DigitCameraChannel

#endif//DIGITAL_CAMERA_LIVE_STREAM_H

