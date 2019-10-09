// devicehost.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <process.h>
#include <io.h>
#include "boost/algorithm/string.hpp"
#include "boost/bind.hpp"
#include "boost/checked_delete.hpp"
#include "boost/format.hpp"
#include "boost/filesystem/path.hpp"
#include "boost/filesystem/operations.hpp"
#include "boost/make_shared.hpp"
#include "boost/property_tree/ptree.hpp"
using PTREE = boost::property_tree::ptree;
#include "boost/property_tree/ini_parser.hpp"
#include "boost/unordered_map.hpp"
#ifdef _WINDOWS
#include "glog/log_severity.h"
#include "glog/logging.h"
#else
#include "glog/log_severity.h"
#include "glog/logging.h"
#endif//_WINDOWS
#include "error.h"
#include "Hardware/Cpu.h"
using CPU = NS(hardware, 1)::Cpu;
#include "MessageQueue/MQContext.h"
using MQContext = NS(mq, 1)::MQContext;
#include "MQModel/Publisher/PublisherModel.h"
using PublisherModel = NS(model, 1)::PublisherModel;
#include "AsynchronousServer.h"
using MQModel = NS(model, 1)::MQModel;
#include "File/Com/ComPort.h"
using ComPort = NS(com, 1)::ComPort;
#include "AlarmMessage.h"
#include "Hikvision7xxxNVR.h"
using NVRDevicePtr = boost::shared_ptr<NS(device, 1)::Device>;
using NVRDeviceGroup = boost::unordered_map<const std::string, NVRDevicePtr>;
#include "DigitCameraLivestream.h"
using AVStreamPtr = boost::shared_ptr<NS(stream, 1)::AVStream>;
using AVStreamGroup = boost::unordered_map<const std::string, AVStreamPtr>;
#include "devicehost.h"

boost::shared_ptr<MQModel> publisherModelPtr;
boost::shared_ptr<MQModel> routerModelPtr;
boost::mutex publishMtx;
ComPort* comPortController[2]{ NULL };//0-clock, 1-AIS
std::string clockAsyncData;
long long clockUTCTime = 0;
std::string aisAsyncData;
int sailingStatus = 0;//0 : sail, 1 : port
int autoCheckSailOrPort = 1;//0 : manual, 1 : auto
int largestRegisterFaceID = 0;
NVRDeviceGroup NVRDevices;
AVStreamGroup livestreams;

int createNewNVRDevice(
	const std::string address, const unsigned short port, 
	const std::string name, const std::string password,
	std::vector<Camera>& digitCameras)
{
	int status{ERR_OK};
	NVRDeviceGroup::iterator it = NVRDevices.find(address);

	if(it != NVRDevices.end())
	{
		status = ERR_EXISTED;
	}
	else
	{
		NVRDevicePtr newNVRDevicePtr{ 
			boost::make_shared<Hikvision7xxxNVR>(name.c_str(), password.c_str(), address.c_str(), port) };
		if (newNVRDevicePtr)
		{
			status = newNVRDevicePtr->createDevice();

			if (ERR_OK == status)
			{
				boost::shared_ptr<Hikvision7xxxNVR> newHikvision7xxxNVRDevicePtr{
					boost::dynamic_pointer_cast<Hikvision7xxxNVR>(newNVRDevicePtr) };
				const int loginUserID{newHikvision7xxxNVRDevicePtr->getUserID()};
				digitCameras = newHikvision7xxxNVRDevicePtr->getDigitCameras();

				NVRDevices.insert(std::make_pair(address, newNVRDevicePtr));
				LOG(INFO) << "Login HIKVISION NVR device [ " << address << "_" << digitCameras.size() << " ], user ID [ " << loginUserID << " ].";
			}
			else
			{
				newNVRDevicePtr->destoryDevice();
				LOG(WARNING) << "Login HIKVISION NVR device [ " << address << " ], failed.";
			}
		}
		else
		{
			LOG(ERROR) << "Create new HIKVISION NVR device failed.";
			status = ERR_BAD_ALLOC;
		}
	}

	return status;
}

int destroyNVRDevice(const std::string address)
{
	int status{ ERR_OK };
	NVRDeviceGroup::iterator it = NVRDevices.find(address);

	if (it == NVRDevices.end())
	{
		LOG(WARNING) << "Can not found NVR device [ " << address << " ].";
		status = ERR_NOT_FOUND;
	}
	else
	{
		for (AVStreamGroup::iterator it = livestreams.begin(); it != livestreams.end();)
		{
			if (std::strstr(it->first.c_str(), address.c_str()))
			{
				status = it->second->closeStream();
				LOG(WARNING) << "Close live stream [ " << it->first << " ] before destroying NVR device.";
				it = livestreams.erase(it);
			} 
			else
			{
				++it;
			}
		}

		status = it->second->destoryDevice();
		NVRDevices.erase(it);
		LOG(WARNING) << "Destroy and Remove NVR device [ " << address << " ].";
	}

	return status;
}

int createNewDigitCamera(
	const std::string NVRAddress, const unsigned long long cameraIndex/* = 0*/, const unsigned int abilities/* = 0*/)
{
	int status{ ERR_NOT_FOUND };
	NVRDeviceGroup::const_iterator cit = NVRDevices.find(NVRAddress);

	if (NVRDevices.end() != cit)
	{
		const std::string livestreamID{
			(boost::format("%s_%d") % NVRAddress % cameraIndex).str() };
		AVStreamGroup::iterator it = livestreams.find(livestreamID);

		if (livestreams.end() == it)
		{
			boost::shared_ptr<NS(device, 1)::HikvisionDevice> hikvisionDevicePtr{
				boost::dynamic_pointer_cast<NS(device, 1)::HikvisionDevice>(cit->second) };
			AVStreamPtr livestreamPtr{ 
				boost::make_shared<DigitCameraLivestream>(
					publisherModelPtr, NVRAddress, hikvisionDevicePtr->getUserID(), cameraIndex) };

			if (livestreamPtr)
			{
				boost::shared_ptr<DigitCameraLivestream> digitCameraLivestreamPtr{
					boost::dynamic_pointer_cast<DigitCameraLivestream>(livestreamPtr) };
				if (digitCameraLivestreamPtr)
				{
					digitCameraLivestreamPtr->setArithmeticAbilities(abilities);
				}

				status = livestreamPtr->openStream();
				if (ERR_OK == status)
				{
					livestreams.insert(std::make_pair(livestreamID, livestreamPtr));
					LOG(INFO) << "Add live stream [ " << livestreamID << " (" << abilities << ") ].";
				}
				else
				{
					LOG(WARNING) << "Add live stream [ " << livestreamID << " (" << abilities << ") ] failed.";
				}
			}
		}
		else
		{
			boost::shared_ptr<DigitCameraLivestream> livestreamPtr{
				boost::dynamic_pointer_cast<DigitCameraLivestream>(it->second) };
			if (livestreamPtr)
			{
				livestreamPtr->setArithmeticAbilities(abilities);
				LOG(INFO) << "Set live stream [ " << livestreamID << " ] arithmetic abilities (" << abilities <<" ).";
			}
		}
	}
	else
	{
		LOG(ERROR) << "Can not create new digit camera [ " << NVRAddress << "_" << cameraIndex << " ].";
	}

	return status;
}

int destroyDigitCamera(
	const std::string NVRAddress, const unsigned long long cameraIndex/* = 0*/)
{
	int status{ ERR_NOT_FOUND };
	NVRDeviceGroup::const_iterator cit = NVRDevices.find(NVRAddress);

	if (NVRDevices.end() != cit)
	{
		const std::string livestreamID{ 
			(boost::format("%s_%d") % NVRAddress % cameraIndex).str() };
		AVStreamGroup::iterator it{ livestreams.find(livestreamID) };

		if (livestreams.end() != it)
		{
			status = it->second->closeStream();
			livestreams.erase(it);
			LOG(INFO) << "Remove live stream [ " << livestreamID << " ].";
		}
		else
		{
			LOG(WARNING) << "Remove live stream [ " << livestreamID << " ] failed.";
		}
	}
	else
	{
		LOG(ERROR) << "Can not destroy digit camera [ " << NVRAddress << "_" << cameraIndex << " ].";
	}

	return status;
}

MediaImagePtr captureDigitCameraLivePicture(
	const std::string NVRAddress, const unsigned long long cameraIndex/* = 0*/)
{
	MediaImagePtr captureImagePtr{ 
		boost::make_shared<MediaImage>(NS(frame, 1)::MediaImageType::MEDIA_IMAGE_JPEG) };

	if (captureImagePtr)
	{
		const std::string cameraID{ 
			(boost::format("%s_%d") % NVRAddress % cameraIndex).str() };
		AVStreamGroup::iterator it{ livestreams.find(cameraID) };

		if (livestreams.end() != it)
		{
			const int jpegPictureBytes = 1024 * 1024;
			char* jpegPictureData{ new(std::nothrow) char[jpegPictureBytes] };

			if (jpegPictureData)
			{
				const unsigned long long capturePictureBytes{ 
					it->second->capturePicture(jpegPictureData, jpegPictureBytes) };

				if (0 < capturePictureBytes)
				{
					captureImagePtr->setImage((const unsigned char*)jpegPictureData, capturePictureBytes);
					LOG(INFO) << "Capture live picture [ " << NVRAddress << "_" << cameraIndex << " ] bytes ( " << capturePictureBytes << " ).";
				}
				else
				{
					LOG(WARNING) << "Capture live picture [ " << NVRAddress << "_" << cameraIndex << " ] failed.";
				}

				boost::checked_array_delete(jpegPictureData);
			}
			else
			{
				LOG(ERROR) << "Bad alloc memory while capturing live picture [ " << NVRAddress << "_" << cameraIndex << " ].";
			}
		}
		else
		{
			LOG(WARNING) << "Can not find camera while capturing live picture [ " << NVRAddress << "_" << cameraIndex << " ].";
		}
	} 
	else
	{
		LOG(ERROR) << "Bad alloc memory while capturing live picture [ " << NVRAddress << "_" << cameraIndex << " ].";
	}

	return captureImagePtr;
}

int setAutoCheckSailOrPort(const int autoCheck/* = 1*/)
{
	autoCheckSailOrPort = autoCheck;
	LOG(INFO) << "Set auto check sail or port flag (0 : manual, 1 : auto) [ " << autoCheckSailOrPort << " ].";
	return ERR_OK;
}

int setSailingStatus(const int status/* = 0*/)
{
	sailingStatus = status;
	LOG(INFO) << "Set sailing status flag (0 : sailing, 1 : porting) " << sailingStatus;
	return ERR_OK;
}

int getSailingStatus(void)
{
//	LOG(INFO) << "Get sailing status flag (0 : sailing, 1 : porting) " << sailingStatus;
	return sailingStatus;
}

int createNewFacePicture(
	const char* imageData /*= NULL*/, const int imageBytes /*= 0*/, const char* name /*= NULL*/, const long long uuid /*= 0*/)
{
	int status{ ERR_INVALID_PARAM };

	if (imageData && imageBytes && name && 0 < uuid)
	{
		const std::string executePath{
				boost::filesystem::initial_path<boost::filesystem::path>().string() };
		const std::string jpegFileName{
			(boost::format("%s\\Face\\%lld_%lld_%s.jpg") % executePath % ++largestRegisterFaceID % uuid % name).str() };

		FILE* fd{ NULL };
		fopen_s(&fd, jpegFileName.c_str(), "wb+");
		if (fd)
		{
			fwrite(imageData, imageBytes, 1, fd);
			fclose(fd);

			for (AVStreamGroup::iterator it = livestreams.begin(); it != livestreams.end(); ++it)
			{
				boost::shared_ptr<DigitCameraLivestream> livestreamPtr{ 
					boost::dynamic_pointer_cast<DigitCameraLivestream>(it->second) };
				if (livestreamPtr)
				{
					status = livestreamPtr->addFacePicture(jpegFileName.c_str(), largestRegisterFaceID);
					LOG(INFO) << "Add face picture [ " << largestRegisterFaceID << "_" << uuid << "_" << name << " ].";
				}
			}
		}
		else
		{
			LOG(WARNING) << "Can not write face picture file [ " << largestRegisterFaceID << "_" << uuid << "_" << name << " ].";
		}
	}
	else
	{
		LOG(WARNING) << "Can not add face picture [ " << largestRegisterFaceID << "_" << uuid << "_" << name << " ].";
	}

	return status;
}

int queryFacePicture(const long long uuid, char*& imageData, int& imageBytes)
{
	const std::string executePath{
				boost::filesystem::initial_path<boost::filesystem::path>().string() };
	boost::filesystem::path recursiveDirPath((boost::format("%s\\Face") % executePath).str());
	boost::filesystem::recursive_directory_iterator endIter;

	for (boost::filesystem::recursive_directory_iterator it(recursiveDirPath); it != endIter; ++it)
	{
		if (!boost::filesystem::is_directory(*it))
		{
			const std::string faceImageFileName{ it->path().filename().string() };

			if (!faceImageFileName.empty())
			{
				std::vector<std::string> faceImageFileNameSegment;
				boost::split(faceImageFileNameSegment, faceImageFileName, boost::is_any_of("_"));
				const int currentUUID{ atoi(faceImageFileNameSegment[1].c_str()) };

				if (uuid == currentUUID)
				{
					FILE* fd{ NULL };
					fopen_s(&fd, faceImageFileName.c_str(), "rb+");

					if (fd)
					{
						imageBytes = _filelength(_fileno(fd));
						fread(imageData, imageBytes, 1, fd);
						fclose(fd);

						LOG(INFO) << "Query face image " << faceImageFileName << "[ " << imageBytes << " Bytes ].";
					}

					break;
				}
			}
		}
	}

	return 0;
}

static void clockTimeUpdateNotifyHandler(const char* data = NULL, const int dataBytes = 0)
{
	if ('$' == *data)
	{
		if (!clockAsyncData.empty())
		{
			std::vector<std::string> clockDatas;
			boost::split(clockDatas, clockAsyncData, boost::is_any_of(","));

			if (7 == clockDatas.size())
			{
				clockUTCTime = atoll(clockDatas[1].c_str());
			}

			clockAsyncData.clear();
		}
	}

	clockAsyncData.append(data, dataBytes);
}

static void AISStatusUpdateNotifyHandler(const char* data = NULL, const int dataBytes = 0)
{
	if ('$' == *data)
	{
		if (!clockAsyncData.empty())
		{
			std::vector<std::string> aisDatas;
			boost::split(aisDatas, aisAsyncData, boost::is_any_of(","));

			if (14 == aisDatas.size() && 1 == sailingStatus)
			{
				sailingStatus = atoi(aisDatas[2].c_str());
			}

			aisAsyncData.clear();
		}
	}

	aisAsyncData.append(data, dataBytes);
}

static bool initSerialPort()
{
	bool clockStatus{ false }, aisStatus{ false };

	try
	{
		comPortController[0] = new ComPort(boost::bind(&clockTimeUpdateNotifyHandler, _1, _2));
		comPortController[1] = new ComPort(boost::bind(&AISStatusUpdateNotifyHandler, _1, _2));

//		for (int i = 0; i != 16; i++)
		{
			if (!clockStatus && ERR_OK == comPortController[0]->initPort(6, CBR_4800))
			{
				clockStatus = true;
				LOG(INFO) << "Open clock port access at COM" << 6;
//				continue;
			}

			if (!aisStatus && ERR_OK == comPortController[1]->initPort(7, CBR_38400))
			{
				aisStatus = true;
				LOG(INFO) << "Open AIS port access at COM" << 7;
			}
		}
	}
	catch (std::exception*)
	{
		boost::checked_delete(comPortController[0]);
		boost::checked_delete(comPortController[1]);
	}

	return clockStatus && aisStatus;
}

static DWORD WINAPI notifyStartingProcessThread(void* ctx = NULL)
{
	while (1)
	{
		char startingNotify[16]{ 0 };
		int* messageID{ (int*)(startingNotify + 8) };
		*messageID = 21;
		boost::shared_ptr<PublisherModel> publisherModel{
			boost::dynamic_pointer_cast<PublisherModel>(publisherModelPtr) };
		publishMtx.lock();
		publisherModel->send(startingNotify, 16);
		publishMtx.unlock();

		Sleep(10000);
	}
}

int main(int argc, char* argv[])
{
	FLAGS_stderrthreshold = GLOG_INFO;
	FLAGS_colorlogtostderr = 1;
	google::InitGoogleLogging(argv[0]);
	google::SetLogDestination(GLOG_INFO,
#ifdef _WINDOWS
		".\\"
#else
		"./"
#endif
	);

	initSerialPort();
	MQContext ctx;

	try
	{
		PTREE rootNode;
		boost::property_tree::ini_parser::read_ini("config.ini", rootNode);

		int publisherPortNumber{ rootNode.get_child("Publisher").get<int>("Port") }, 
			routerPortNumber{ rootNode.get_child("Router").get<int>("Port") }, 
			cpuCoreNumber{ CPU().getCPUCoreNumber() };
		ctx.initialize(cpuCoreNumber);
		boost::shared_ptr<MQModel> publisherPtr{ 
			boost::make_shared<PublisherModel>(publisherPortNumber) };
		boost::shared_ptr<MQModel> routerPtr{ boost::make_shared<AsynchronousServer>(routerPortNumber) };

		if (publisherPtr && routerPtr)
		{
			publisherModelPtr.swap(publisherPtr);
			routerModelPtr.swap(routerPtr);

			if (ERR_OK == publisherModelPtr->start(ctx) && ERR_OK == routerModelPtr->start(ctx))
			{
				LOG(INFO) << "Start publisher port number " << publisherPortNumber << 
					" and router port number " << routerPortNumber << 
					" and CPU core number " << cpuCoreNumber << ".";
			}
		}
		else
		{
			LOG(ERROR) << "Failed to create publisher and router service.";
		}

		DWORD threadID{ 0 };
		CreateThread(NULL, 0, &notifyStartingProcessThread, NULL, 0, &threadID);
		getchar();

		if (publisherModelPtr)
		{
			publisherModelPtr->stop(ctx);
			publisherModelPtr.reset();
		}

		if (routerModelPtr)
		{
			routerModelPtr->stop(ctx);
			routerModelPtr.reset();
		}

		ctx.deinitialize();
	}
	catch (std::exception& e)
	{
		LOG(ERROR) << e.what();
	}

	google::ShutdownGoogleLogging();
	return 0;
}
