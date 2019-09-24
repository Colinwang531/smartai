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
int autoCheckSwitch = 1;//0 : manual, 1 : auto

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

// 	if (frame && 0 < detectInfos.size())
// 	{
// 		AlarmMessageData* alarmMessageData = new AlarmMessageData;
// 		if (alarmMessageData)
// 		{
// 			alarmMessageData->frame = frame;
// 			alarmMessageData->detectNotify = detectInfos;
// 			alarmMessageQueue->pushBack(alarmMessageData);
// 		}
// 	}
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

		bool status = helmetAlgorithmPtr->initialize(exePath.c_str(), 0.25f, 0.15f);
		if (status)
		{
			LOG(INFO) << "Initialize HELMET algorithm status Successfully.";
		}
		else
		{
			LOG(WARNING) << "Initialize HELMET algorithm status Failed.";
		}

		/*bool*/ status = sleepAlgorithmPtr->initialize(exePath.c_str(), 0.2f, 0.15f);
		if (status)
		{
			LOG(INFO) << "Initialize SLEEP algorithm status Successfully.";
		}
		else
		{
			LOG(WARNING) << "Initialize SLEEP algorithm status Failed.";
		}

		/*bool*/ status = phoneAlgorithmPtr->initialize(exePath.c_str(), 0.9f, 0.2f);
		if (status)
		{
			LOG(INFO) << "Initialize PHONE algorithm status Successfully.";
		}
		else
		{
			LOG(WARNING) << "Initialize PHONE algorithm status Failed.";
		}

		status = fightAlgorithmPtr->initialize(exePath.c_str(), 0.995f, 0.2f);
		if (status)
		{
			LOG(INFO) << "Initialize FIGHT algorithm status Successfully.";
		}
		else
		{
			LOG(WARNING) << "Initialize FIGHT algorithm status Failed.";
		}

		/*bool*/ status = faceAlgorithmPtr->initialize(exePath.c_str(), 0.3f, 0.15f);
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

//		stopped = true;
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
