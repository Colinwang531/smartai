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
boost::shared_ptr<CVAlgo> cvAlgoHelmetPtr;
boost::shared_ptr<CVAlgo> cvAlgoSleepPtr;
boost::shared_ptr<CVAlgo> cvAlgoPhonePtr;
boost::shared_ptr<CVAlgo> cvAlgoFacePtr;
boost::shared_ptr<CVAlgo> cvAlgoFightPtr;
FIFOList* bgr24FrameQueue[NS(algo, 1)::AlgoType::ALGO_NONE]{ NULL };
ComPort* comPortController[2]{ NULL };//0-clock, 1-AIS
std::string clockAsyncData;
long long clockUTCTime = 0;
std::string aisAsyncData;
int sailingStatus = 1;
int autoSailingStatusCheck = 1;

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

			if (14 == aisDatas.size() && 1 == autoSailingStatusCheck)
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
		publisherModel->send(message.getMessageData(), message.getMessageBytes());
		LOG(INFO) << "Send push alarm " << detectInfos[0].type;
	}
}

static void initAlgo(void)
{
	const std::string exePath{ 
		boost::filesystem::initial_path<boost::filesystem::path>().string() };

	try
	{
		for (int i = 0; i != NS(algo, 1)::AlgoType::ALGO_NONE; ++i)
		{
			bgr24FrameQueue[i] = new FIFOList(25);
		}

		boost::shared_ptr<CVAlgo> helmetPtr{
			boost::make_shared<CVAlgoHelmet>(
				sailingStatus, bgr24FrameQueue[NS(algo, 1)::AlgoType::ALGO_HELMET], boost::bind(&cvAlgoDetectInfoHandler, _1, _2))};
		boost::shared_ptr<CVAlgo> phonePtr{
			boost::make_shared<CVAlgoPhone>(
				sailingStatus, bgr24FrameQueue[NS(algo, 1)::AlgoType::ALGO_PHONE], boost::bind(&cvAlgoDetectInfoHandler, _1, _2)) };
		boost::shared_ptr<CVAlgo> sleepPtr{
			boost::make_shared<CVAlgoSleep>(
				sailingStatus, bgr24FrameQueue[NS(algo, 1)::AlgoType::ALGO_SLEEP], boost::bind(&cvAlgoDetectInfoHandler, _1, _2))};
		boost::shared_ptr<CVAlgo> fightPtr{
			boost::make_shared<CVAlgoFight>(
				sailingStatus, bgr24FrameQueue[NS(algo, 1)::AlgoType::ALGO_FIGHT], boost::bind(&cvAlgoDetectInfoHandler, _1, _2))};
		boost::shared_ptr<CVAlgo> facePtr{
			boost::make_shared<CVAlgoFace>(
				sailingStatus, bgr24FrameQueue[NS(algo, 1)::AlgoType::ALGO_FACE], boost::bind(&cvAlgoDetectInfoHandler, _1, _2))};

		if (helmetPtr && sleepPtr && phonePtr && fightPtr && facePtr)
		{
			cvAlgoHelmetPtr.swap(helmetPtr);
			cvAlgoSleepPtr.swap(sleepPtr);
			cvAlgoPhonePtr.swap(phonePtr);
			cvAlgoFightPtr.swap(fightPtr);
			cvAlgoFacePtr.swap(facePtr);

// 			bool status = cvAlgoHelmetPtr->initialize(exePath.c_str(), 0.25f, 0.15f);
// 			if (status)
// 			{
// 				LOG(INFO) << "Initialize HELMET algorithm status Successfully.";
// 			} 
// 			else
// 			{
// 				LOG(WARNING) << "Initialize HELMET algorithm status Failed.";
// 			}
// 
// 			status = cvAlgoSleepPtr->initialize(exePath.c_str(), 0.2f, 0.15f);
// 			if (status)
// 			{
// 				LOG(INFO) << "Initialize SLEEP algorithm status Successfully.";
// 			}
// 			else
// 			{
// 				LOG(WARNING) << "Initialize SLEEP algorithm status Failed.";
// 			}

// 			status = cvAlgoPhonePtr->initialize(exePath.c_str(), 0.9f, 0.2f);
// 			if (status)
// 			{
// 				LOG(INFO) << "Initialize PHONE algorithm status Successfully.";
// 			}
// 			else
// 			{
// 				LOG(WARNING) << "Initialize PHONE algorithm status Failed.";
// 			}

// 			status = cvAlgoFightPtr->initialize(exePath.c_str(), 0.995f, 0.2f);
// 			if (status)
// 			{
// 				LOG(INFO) << "Initialize FIGHT algorithm status Successfully.";
// 			}
// 			else
// 			{
// 				LOG(WARNING) << "Initialize FIGHT algorithm status Failed.";
// 			}

			bool status = cvAlgoFacePtr->initialize(exePath.c_str());
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
	catch(std::exception*)
	{
		for (int i = 0; i != NS(algo, 1)::AlgoType::ALGO_NONE; ++i)
		{
			boost::checked_delete(bgr24FrameQueue[i]);
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

		for (int i = 0; i != 16; i++)
		{
			if (!clockStatus && ERR_OK == comPortController[0]->initPort(i, CBR_4800))
			{
				clockStatus = true;
				LOG(INFO) << "Open clock port access at COM" << i;
			}

			if (!aisStatus && comPortController[1]->initPort(i, CBR_38400))
			{
				aisStatus = true;
				LOG(INFO) << "Open AIS port access at COM" << i;
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
	initAlgo();
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
		boost::shared_ptr<MQModel> routerPtr{ 
			boost::make_shared<AsynchronousServer>(cvAlgoFacePtr, routerPortNumber, bgr24FrameQueue) };

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
