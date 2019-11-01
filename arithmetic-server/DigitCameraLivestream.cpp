#include "boost/bind.hpp"
#include "boost/filesystem/path.hpp"
#include "boost/filesystem/operations.hpp"
#include "boost/make_shared.hpp"
#include "boost/algorithm/string.hpp"
#ifdef _WINDOWS
#include "glog/log_severity.h"
#include "glog/logging.h"
#else
#include "glog/log_severity.h"
#include "glog/logging.h"
#endif//_WINDOWS
#include "error.h"
#include "MQModel/Publisher/PublisherModel.h"
using PublisherModel = NS(model, 1)::PublisherModel;
#include "MediaDecoder/SDK/HikvisionSDKDecoder.h"
using HikvisionSDKDecoder = NS(decoder, 1)::HikvisionSDKDecoder;
#include "MediaConverter/YV12ToYUV420PConverter.h"
using YV12ToYUV420PConverter = NS(converter, 1)::YV12ToYUV420PConverter;
#include "MediaConverter/YUV420PToBGR24Converter.h"
using YUV420PToBGR24Converter = NS(converter, 1)::YUV420PToBGR24Converter;
#include "MediaConverter/BGR24ToYUV420PConverter.h"
using BGR24ToYUV420PConverter = NS(converter, 1)::BGR24ToYUV420PConverter;
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
#include "ArithmeticServer.h"
#include "DigitCameraLivestream.h"

extern int sailingStatus;//0 : sail, 1 : port
extern int autoCheckSwitch;//0 : manual, 1 : auto
extern int getThreadAffinityMask(void);
extern enum class tagWorkMode_t serverWorkMode;

DigitCameraLivestream::DigitCameraLivestream(
	boost::shared_ptr<MQModel> publisher, const std::string NVRIp,
	const long uid /* = -1 */, const unsigned short idx /* = -1 */)
	: HikvisionLivestream(uid, idx), arithmeticAbilities{ 0 }, livestreamFrameNumber{ 0 }, NVRIpAddress{ NVRIp }, publisherModelPtr{ publisher }
{}

DigitCameraLivestream::~DigitCameraLivestream()
{}

int DigitCameraLivestream::openStream()
{
	int status{ ERR_BAD_ALLOC };
	boost::shared_ptr<MediaDecoder> videoDecoderPtr{ 
		boost::make_shared<HikvisionSDKDecoder>(
			boost::bind(&DigitCameraLivestream::videoStreamDecodeHandler, this, _1, _2, _3, _4)) };
	boost::shared_ptr<MediaConverter> yv12ToYuv420pConverter{ boost::make_shared<YV12ToYUV420PConverter>() };
	boost::shared_ptr<MediaConverter> yuv420pToBGR24Converter{ boost::make_shared<YUV420PToBGR24Converter>() };
	boost::shared_ptr<MediaConverter> bgr24ToYuv420pConverter{ boost::make_shared<BGR24ToYUV420PConverter>() };
	boost::shared_ptr<MediaEncoder> jpegEncoderPtr{ boost::make_shared<YUV420PToJPEGEncoder>() };

	if (videoDecoderPtr && yv12ToYuv420pConverter && yuv420pToBGR24Converter && bgr24ToYuv420pConverter && jpegEncoderPtr)
	{
		h264LivestreamQueue.setCapacity(1000);
		videoStreamDecoderPtr.swap(videoDecoderPtr);
		yv12ToYuv420pConverterPtr.swap(yv12ToYuv420pConverter);
		yv12ToYuv420pConverterPtr->initialize();
		yuv420pToBGR24ConverterPtr.swap(yuv420pToBGR24Converter);
		yuv420pToBGR24ConverterPtr->initialize();
		bgr24ToYuv420pConverterPtr.swap(bgr24ToYuv420pConverter);
		bgr24ToYuv420pConverterPtr->initialize();
		jpegPictureEncoderPtr.swap(jpegEncoderPtr);
		jpegPictureEncoderPtr->initialize();

		status = HikvisionLivestream::openStream();
		if (ERR_OK == status)
		{
			DWORD threadID{ 0 };
			HANDLE handle{ CreateThread(NULL, 0, &DigitCameraLivestream::frameDecodeProcessThread, this, 0, &threadID) };
			if (handle)
			{
// 				SetThreadPriority(handle, THREAD_PRIORITY_TIME_CRITICAL/*THREAD_PRIORITY_HIGHEST*/);
// 				SetThreadAffinityMask(handle, getThreadAffinityMask());
			}
		}
	}

	return status;
}

int DigitCameraLivestream::closeStream()
{
	int status{ HikvisionLivestream::closeStream() };

	if (ERR_OK == status)
	{
		boost::unique_lock<boost::mutex> lock1{ mtx[0] }, lock2{ mtx[1] }, lock3{ mtx[2] };
		condition[0].wait_for(lock1, boost::chrono::seconds(3));
		condition[1].wait_for(lock2, boost::chrono::seconds(3));
		condition[2].wait_for(lock3, boost::chrono::seconds(3));

		if (yv12ToYuv420pConverterPtr)
		{
			yv12ToYuv420pConverterPtr->deinitialize();
		}
		
		if (yuv420pToBGR24ConverterPtr)
		{
			yuv420pToBGR24ConverterPtr->deinitialize();
		}

		if (bgr24ToYuv420pConverterPtr)
		{
			bgr24ToYuv420pConverterPtr->deinitialize();
		}
		
		if (jpegPictureEncoderPtr)
		{
			jpegPictureEncoderPtr->deinitialize();
		}

		while (1)
		{
			MediaImagePtr imagePtr{ h264LivestreamQueue.remove() };
			if (!imagePtr)
			{
				break;
			}
		}
	}

	return status;
}


void DigitCameraLivestream::setArithmeticAbilities(const unsigned int abilities/* = 0*/, void* parameter/* = NULL*/)
{
	arithmeticAbilities = abilities;
	const std::string exePath{
		boost::filesystem::initial_path<boost::filesystem::path>().string() };

	//printf("Set arithmetic %d abilities %d.\r\n", streamIndex, abilities);

	if (arithmeticAbilities & 0x01)
	{
		if (!helmetArithmeticPtr)
		{
			CVAlgoPtr helmet{ 
				boost::make_shared<CVAlgoHelmet>(NVRIpAddress, streamIndex,
					boost::bind(&DigitCameraLivestream::alarmInfoProcessHandler, this, _1, _2)) };
			if (helmet)
			{
				if (helmet->initialize(exePath.c_str(), getThreadAffinityMask(), 0.20f, 0.15f, (int)serverWorkMode))
				{
					helmetArithmeticPtr.swap(helmet);
					LOG(INFO) << "Initialize HELMET arithmetic Successfully, GPU = [ " << (int)serverWorkMode << " ].";
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
			helmetArithmeticPtr->deinitialize();
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
				if (phone->initialize(exePath.c_str(), getThreadAffinityMask(), 0.80f, 0.20f, (int)serverWorkMode))
				{
					phoneArithmeticPtr.swap(phone);
					LOG(INFO) << "Initialize PHONE arithmetic Successfully, GPU = [ " << (int)serverWorkMode << " ].";
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
			phoneArithmeticPtr->deinitialize();
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
				if (sleep->initialize(exePath.c_str(), getThreadAffinityMask(), 0.65f, 0.15f, (int)serverWorkMode))
				{
					sleepArithmeticPtr.swap(sleep);
					LOG(INFO) << "Initialize SLEEP arithmetic Successfully, GPU = [ " << (int)serverWorkMode << " ].";
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
			sleepArithmeticPtr->deinitialize();
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
				if (fight->initialize(exePath.c_str(), getThreadAffinityMask(), 0.99f, 0.10f, (int)serverWorkMode))
				{
					fightArithmeticPtr.swap(fight);
					LOG(INFO) << "Initialize FIGHT arithmetic Successfully, GPU = [ " << (int)serverWorkMode << " ].";
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
			fightArithmeticPtr->deinitialize();
			fightArithmeticPtr.reset();
		}
	}

	if ((arithmeticAbilities >> 4) & 0x01)
	{
		if (!faceArithmeticPtr)
		{
			CVAlgoPtr face{ 
				boost::make_shared<CVAlgoFace>(
					boost::bind(&DigitCameraLivestream::faceInfoProcessHandler, this, _1, _2)) };
			if (face)
			{
				if (face->initialize(exePath.c_str(), getThreadAffinityMask(), 0.20f, 0.15f, (int)serverWorkMode))
				{
					std::vector<std::string>* faceImages{ reinterpret_cast<std::vector<std::string>*>(parameter) };
					boost::shared_ptr<CVAlgoFace> algoFacePtr{ boost::dynamic_pointer_cast<CVAlgoFace>(face) };

					for (std::vector<std::string>::iterator it = faceImages->begin(); it != faceImages->end(); ++it)
					{
						const int pos{ (const int)(*it).rfind("\\") };
						const std::string fileName{ (*it).substr(pos + 1, (*it).length()) };
						std::vector<std::string> faceImageFileNameSegment;
						boost::split(faceImageFileNameSegment, fileName, boost::is_any_of("_"));
						algoFacePtr->addFacePicture((*it).c_str(), atoi(faceImageFileNameSegment[0].c_str()));
					}

					faceArithmeticPtr.swap(face);
					LOG(INFO) << "Initialize FACE arithmetic Successfully, GPU = [ " << (int)serverWorkMode << " ].";
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
			faceArithmeticPtr->deinitialize();
			faceArithmeticPtr.reset();
		}
	}
}

void DigitCameraLivestream::captureVideoStreamProcess(
	const unsigned char* data /* = NULL */, const long long dataBytes /* = 0 */)
{
	if (NS(stream, 1)::AVStreamStatus::AVSTREAM_STATUS_STOP == streamStatus)
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

void DigitCameraLivestream::videoStreamDecodeHandler(
	const char* frameData /* = NULL */, const long frameBytes /* = 0 */, 
	const long imageWidth /* = 0 */, const long imageHeight /* = 0 */)
{
	if (NS(stream, 1)::AVStreamStatus::AVSTREAM_STATUS_STOP == streamStatus)
	{
		condition[1].notify_one();
		return;
	}

	const unsigned int phoneFlag{ (arithmeticAbilities >> 1) & 0x01 }, sleepFlag{ (arithmeticAbilities >> 2) & 0x01 };
	if ((1 == sailingStatus && (0 < phoneFlag || 0 < sleepFlag)) || (1 != (++livestreamFrameNumber % 3)))
	{
		return;
	}

	if (yv12ToYuv420pConverterPtr && yuv420pToBGR24ConverterPtr)
	{
		const long imageDataBytes{ imageWidth * imageHeight * 3 / 2 };
		const unsigned char* yuv420pImageData{ 
			yv12ToYuv420pConverterPtr->convert(
				(const unsigned char*)frameData, frameBytes, (unsigned short)imageWidth, (unsigned short)imageHeight) };
		const unsigned char* bgr24ImageData{
			yuv420pToBGR24ConverterPtr->convert(yuv420pImageData, imageDataBytes, (const unsigned short)imageWidth, (const unsigned short)imageHeight) };

		if (bgr24ImageData)
		{
			MediaImagePtr bgr24ImagePtr{
				boost::make_shared<NS(frame, 1)::MediaImage>(NS(frame, 1)::MediaImageType::MEDIA_IMAGE_BGR24) };

			if (bgr24ImagePtr)
			{
 				bgr24ImagePtr->setImage(bgr24ImageData, imageWidth * imageHeight * 3);

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
 	}
}

DWORD DigitCameraLivestream::frameDecodeProcessThread(void* ctx /* = NULL */)
{
	DigitCameraLivestream* livestream{ reinterpret_cast<DigitCameraLivestream*>(ctx) };

	while (livestream && livestream->videoStreamDecoderPtr)
	{
		if (NS(stream, 1)::AVStreamStatus::AVSTREAM_STATUS_STOP == livestream->streamStatus)
		{
			while (livestream->h264LivestreamQueue.remove()){}
			livestream->condition[2].notify_one();
			break;
		}
		else
		{
			MediaImagePtr h264ImagePtr{ livestream->h264LivestreamQueue.remove() };
			if (h264ImagePtr)
			{
				livestream->videoStreamDecoderPtr->decode(h264ImagePtr->getImage(), (int)h264ImagePtr->getImageBytes());
			}
			else
			{
				Sleep(1);
			}
		}
	}

	return 0;
}

void DigitCameraLivestream::alarmInfoProcessHandler(
	MediaImagePtr image, std::vector<NS(algo, 1)::AlarmInfo> alarmInfos)
{
	if (image && 0 < alarmInfos.size() && jpegPictureEncoderPtr && bgr24ToYuv420pConverterPtr)
	{
		const unsigned char* yuv420pImageData{
			bgr24ToYuv420pConverterPtr->convert(image->getOriginImage(), image->getOriginImageBytes(), 1920, 1080) };

		if (yuv420pImageData)
		{
			unsigned char* jpegPictureData{ NULL };
			unsigned long long jpegPictureDataBytes{ 0 };

			if (ERR_OK == jpegPictureEncoderPtr->encode(yuv420pImageData, 1920 * 1080 * 3 / 2))
			{
				jpegPictureEncoderPtr->data(jpegPictureData, jpegPictureDataBytes);

				AlarmMessage message;
				message.setAlarmMessageData(
					(int)alarmInfos[0].type, 1920, 1080, NVRIpAddress.c_str(), streamIndex, alarmInfos, (const char*)jpegPictureData, (int)jpegPictureDataBytes);
				boost::shared_ptr<PublisherModel> publisherModel{
					boost::dynamic_pointer_cast<PublisherModel>(publisherModelPtr) };

				publisherMtx.lock();
				publisherModel->send(message.getMessageData(), message.getMessageBytes());
				publisherMtx.unlock();

				LOG(INFO) << "Send alarm message " << NVRIpAddress << "_" << streamIndex << "_" << (int)alarmInfos[0].type << ".";
			}
		}
	}
}

void DigitCameraLivestream::faceInfoProcessHandler(
	MediaImagePtr image, std::vector<NS(algo, 1)::FaceInfo> faceInfos)
{
	if (image && 0 < faceInfos.size() && jpegPictureEncoderPtr && bgr24ToYuv420pConverterPtr)
	{
		const unsigned char* yuv420pImageData{
			bgr24ToYuv420pConverterPtr->convert(image->getImage(), image->getImageBytes(), 1920, 1080) };

		if (yuv420pImageData)
		{
			unsigned char* jpegPictureData{ NULL };
			unsigned long long jpegPictureDataBytes{ 0 };

			if (ERR_OK == jpegPictureEncoderPtr->encode(yuv420pImageData, 1920 * 1080 * 3 / 2))
			{
				jpegPictureEncoderPtr->data(jpegPictureData, jpegPictureDataBytes);

				AlarmMessage message;
				message.setFaceMessageData(
					(int)faceInfos[0].type, 1920, 1080, NVRIpAddress.c_str(), streamIndex, faceInfos, (const char*)jpegPictureData, (int)jpegPictureDataBytes);
				boost::shared_ptr<PublisherModel> publisherModel{
					boost::dynamic_pointer_cast<PublisherModel>(publisherModelPtr) };

				publisherMtx.lock();
				publisherModel->send(message.getMessageData(), message.getMessageBytes());
				publisherMtx.unlock();

				LOG(INFO) << "Send FACE detected message " << NVRIpAddress << "_" << streamIndex << ".";
			}
		}
	}
}

int DigitCameraLivestream::addFacePicture(const char* filePath /*= NULL*/, const int faceID /*= 0*/)
{
	int status{ ERR_INVALID_PARAM };

	if (filePath && 0 < faceID && faceArithmeticPtr)
	{
		boost::shared_ptr<CVAlgoFace> facePtr{
					boost::dynamic_pointer_cast<CVAlgoFace>(faceArithmeticPtr) };
		status = facePtr ? facePtr->addFacePicture((char*)filePath, faceID) : ERR_BAD_OPERATE;
	}

	return status;
}
