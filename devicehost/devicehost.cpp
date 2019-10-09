// devicehost.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <process.h>
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
#include "Arithmetic/CVAlgoHelmet.h"
#include "Arithmetic/CVAlgoPhone.h"
#include "Arithmetic/CVAlgoSleep.h"
#include "Arithmetic/CVAlgoFace.h"
#include "Arithmetic/CVAlgoFight.h"
using CVAlgo = NS(algo, 1)::CVAlgo;
using CVAlgoHelmet = NS(algo, 1)::CVAlgoHelmet;
using CVAlgoPhone = NS(algo, 1)::CVAlgoPhone;
using CVAlgoSleep = NS(algo, 1)::CVAlgoSleep;
using CVAlgoFace = NS(algo, 1)::CVAlgoFace;
using CVAlgoFight = NS(algo, 1)::CVAlgoFight;
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
#include "Camera/Camera.h"
using Camera = NS(device, 1)::Camera;

boost::shared_ptr<MQModel> publisherModelPtr;
boost::shared_ptr<MQModel> routerModelPtr;
boost::shared_ptr<CVAlgo> helmetAlgorithmPtr;
boost::shared_ptr<CVAlgo> phoneAlgorithmPtr;
boost::shared_ptr<CVAlgo> sleepAlgorithmPtr;
boost::shared_ptr<CVAlgo> fightAlgorithmPtr;
boost::shared_ptr<CVAlgo> faceAlgorithmPtr;
boost::mutex publishMtx;
ComPort* comPortController[2]{ NULL };//0-clock, 1-AIS
std::string clockAsyncData;
long long clockUTCTime = 0;
std::string aisAsyncData;
int sailingStatus = 0;//0 : sail, 1 : port
int autoCheckSailOrPort = 1;//0 : manual, 1 : auto
NVRDeviceGroup NVRDevices;
AVStreamGroup livestreams;

static int createNewNVRDevice(
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

static int destroyNVRDevice(const std::string address)
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
		LOG(WARNING) << "Destroy NVR device [ " << address << " ].";
	}

	return status;
}

static int createNewDigitCamera(
	const std::string NVRAddress, const unsigned long long cameraIndex = 0, const unsigned int abilities = 0)
{
	int status{ ERR_NOT_FOUND };
	NVRDeviceGroup::const_iterator cit = NVRDevices.find(NVRAddress);

	if (NVRDevices.end() != cit)
	{
		const std::string livestreamID{
			(boost::format("%s_%ulld") % NVRAddress % cameraIndex).str() };
		AVStreamGroup::iterator it = livestreams.find(livestreamID);

		if (livestreams.end() == it)
		{
			boost::shared_ptr<NS(device, 1)::HikvisionDevice> hikvisionDevicePtr{
				boost::dynamic_pointer_cast<NS(device, 1)::HikvisionDevice>(cit->second) };
			AVStreamPtr livestreamPtr{ 
				boost::make_shared<DigitCameraLivestream>(hikvisionDevicePtr->getUserID(), cameraIndex) };

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
				LOG(INFO) << "Set live stream [ " << livestreamID << " ] arithmetic abilities [ " << abilities << " ].";
			}
		}
	}
	else
	{
		LOG(ERROR) << "Can not create new digit camera [ " << NVRAddress << "_" << cameraIndex << " ].";
	}

	return status;
}

static int destroyDigitCamera(
	const std::string NVRAddress, const unsigned long long cameraIndex = 0)
{
	int status{ ERR_NOT_FOUND };
	NVRDeviceGroup::const_iterator cit = NVRDevices.find(NVRAddress);

	if (NVRDevices.end() != cit)
	{
		const std::string livestreamID{ 
			(boost::format("%s_%ulld") % NVRAddress % cameraIndex).str() };
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

static int setAutoCheckSailOrPort(const int autoCheck = 1)
{
	autoCheckSailOrPort = autoCheck;
	LOG(INFO) << "Set auto check sail or port flag (0 : manual, 1 : auto) [ " << autoCheckSailOrPort << " ].";
	return ERR_OK;
}

static int setSailingStatus(const int status = 0)
{
	sailingStatus = status;
	LOG(INFO) << "Set sailing status flag (0 : sailing, 1 : porting) " << sailingStatus;
	return ERR_OK;
}

static int getSailingStatus(void)
{
	LOG(INFO) << "Get sailing status flag (0 : sailing, 1 : porting) " << sailingStatus;
	return sailingStatus;
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

			if (14 == aisDatas.size() && 1 == autoCheckSwitch)
			{
				sailingStatus = atoi(aisDatas[2].c_str());
			}

			aisAsyncData.clear();
		}
	}

	aisAsyncData.append(data, dataBytes);
}

static void cvAlgoDetectInfoHandler(void* frame, const std::vector<NS(algo, 1)::DetectNotify> detectInfos)
{
	if (frame && 0 < detectInfos.size())
	{
		BGR24Frame* bgr24Frame{ reinterpret_cast<BGR24Frame*>(frame) };
		AlarmMessage message;
		message.setMessageData(
			detectInfos[0].type, 1920, 1080, bgr24Frame->NVRIp, bgr24Frame->channelIndex, detectInfos, bgr24Frame->jpegData, bgr24Frame->jpegBytes);
		boost::shared_ptr<PublisherModel> publisherModel{
			boost::dynamic_pointer_cast<PublisherModel>(publisherModelPtr) };

		publishMtx.lock();
		publisherModel->send(message.getMessageData(), message.getMessageBytes());
		publishMtx.unlock();

		LOG(INFO) << "Send push alarm " << bgr24Frame->NVRIp << "_" << bgr24Frame->channelIndex << "_" << detectInfos[0].type;
	}
}

static unsigned int alarmMessagePusherHandler(void* ctx)
{
// 	while (!stopped)
// 	{
// 		AlarmMessageData* alarmMessageData{ reinterpret_cast<AlarmMessageData*>(alarmMessageQueue->getFront()) };
// 
// 		if (alarmMessageData)
// 		{
// 			BGR24Frame* bgr24Frame{ reinterpret_cast<BGR24Frame*>(alarmMessageData->frame) };
// 			AlarmMessage message;
// 
// 			message.setMessageData(
// 				alarmMessageData->detectNotify[0].type, 1920, 1080, bgr24Frame->NVRIp,
// 				bgr24Frame->channelIndex, alarmMessageData->detectNotify, bgr24Frame->jpegData, bgr24Frame->jpegBytes);
// 			boost::shared_ptr<PublisherModel> publisherModel{
// 				boost::dynamic_pointer_cast<PublisherModel>(publisherModelPtr) };
// 			publisherModel->send(message.getMessageData(), message.getMessageBytes());
// 			LOG(INFO) << "Send push alarm " << bgr24Frame->NVRIp << "_" << bgr24Frame->channelIndex << "_" << alarmMessageData->detectNotify[0].type;
// 	}

	return 0;
}

static void initAlgorithm(void)
{
	const std::string exePath{ 
		boost::filesystem::initial_path<boost::filesystem::path>().string() };

	boost::shared_ptr<CVAlgo> helmetPtr{
			boost::make_shared<CVAlgoHelmet>(boost::bind(&cvAlgoDetectInfoHandler, _1, _2)) };
	boost::shared_ptr<CVAlgo> phonePtr{
		boost::make_shared<CVAlgoPhone>(boost::bind(&cvAlgoDetectInfoHandler, _1, _2)) };
	boost::shared_ptr<CVAlgo> sleepPtr{
		boost::make_shared<CVAlgoSleep>(boost::bind(&cvAlgoDetectInfoHandler, _1, _2)) };
	boost::shared_ptr<CVAlgo> fightPtr{
		boost::make_shared<CVAlgoFight>(boost::bind(&cvAlgoDetectInfoHandler, _1, _2)) };
	boost::shared_ptr<CVAlgo> facePtr{
		boost::make_shared<CVAlgoFace>(boost::bind(&cvAlgoDetectInfoHandler, _1, _2)) };

	if (helmetPtr && sleepPtr && phonePtr && fightPtr && facePtr)
	{
		helmetAlgorithmPtr.swap(helmetPtr);
		phoneAlgorithmPtr.swap(phonePtr);
		sleepAlgorithmPtr.swap(sleepPtr);
		fightAlgorithmPtr.swap(fightPtr);
		faceAlgorithmPtr.swap(facePtr);

		bool status = helmetAlgorithmPtr->initialize(exePath.c_str(), /*0.25f*/0.95f, 0.10f);
		if (status)
		{
			LOG(INFO) << "Initialize HELMET algorithm status Successfully.";
		}
		else
		{
			LOG(WARNING) << "Initialize HELMET algorithm status Failed.";
		}

		/*bool*/ status = sleepAlgorithmPtr->initialize(exePath.c_str(), /*0.2f*/0.9f, 0.10f/*5*/);
		if (status)
		{
			LOG(INFO) << "Initialize SLEEP algorithm status Successfully.";
		}
		else
		{
			LOG(WARNING) << "Initialize SLEEP algorithm status Failed.";
		}

		/*bool*/ status = phoneAlgorithmPtr->initialize(exePath.c_str(), 0.9f, /*0.2*/0.10f);
		if (status)
		{
			LOG(INFO) << "Initialize PHONE algorithm status Successfully.";
		}
		else
		{
			LOG(WARNING) << "Initialize PHONE algorithm status Failed.";
		}

		/*bool*/ status = fightAlgorithmPtr->initialize(exePath.c_str(), 0.995f, /*0.2*/0.10f);
		if (status)
		{
			LOG(INFO) << "Initialize FIGHT algorithm status Successfully.";
		}
		else
		{
			LOG(WARNING) << "Initialize FIGHT algorithm status Failed.";
		}

		/*bool*/ status = faceAlgorithmPtr->initialize(exePath.c_str(), /*0.3f*/0.9f, 0.15f);
		if (status)
		{
			LOG(INFO) << "Initialize FACE algorithm status Successfully.";
		}
		else
		{
			LOG(WARNING) << "Initialize FACE algorithm status Failed.";
		}
	}
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

static void initAlarmPusher()
{
//	alarmMessageQueue = new FIFOList(1000);

// 	if (alarmMessageQueue)
// 	{
//		HANDLE handle = (HANDLE)_beginthreadex(NULL, 0, &alarmMessagePusherHandler, NULL, 0, NULL);
//		SetThreadPriority(handle, THREAD_PRIORITY_TIME_CRITICAL);
//		SetThreadAffinityMask(handle, (DWORD)(1 << algoNumber));
//	}
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

	initAlarmPusher();
	initSerialPort();
	initAlgorithm();
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
