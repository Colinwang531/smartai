// devicehost.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <windows.h>
#include <process.h>
#include "boost/bind.hpp"
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
#include "MQModel/Publisher/PublisherModel.h"
using PublisherModel = NS(model, 1)::PublisherModel;
#include "DeviceHostServer.h"
using MQModel = NS(model, 1)::MQModel;
#include "AlarmMessage.h"
#include "BaseHelmetDll.h"
#include "BasePhoneDll.h"
#include "BaseSleepDll.h"

typedef enum tagAIAlgo_t
{
	HELMET = 0,
	PHONE,
	SLEEP,
	FIGHT,
	OFF_DUTY,
	ALGO_NONE
}AIAlgo;

typedef struct tagAIAlgoContext_t
{
	AIAlgo algo;
}AIAlgoContext;

AIAlgoContext algoContext[ALGO_NONE];
boost::shared_ptr<MQModel> publisherModelPtr;
boost::shared_ptr<MQModel> routerModelPtr;
//All live view channels are here.
std::vector<boost::shared_ptr<Livestream>> livestreamGroup;
CHelmetAlgorithmClass helmetDetectAlgo;
CPhoneAlgorithmClass phoneDectectAlgo;
CSleepAlgorithmClass sleepDectectAlgo;
BGR24FrameCache bgr24FrameCaches[ALGO_NONE];
int helmetMessageNumber = 0, phoneMessageNumber = 0, sleepMessageNumber = 0;

static void sleepDectctAlgo(const int w = 1920, const int h = 1080, const int channel = 3)
{
	while (1)
	{
		BGR24Frame bgr24Frame;
		bool status{ bgr24FrameCaches[SLEEP].front(bgr24Frame) };

		if (status)
		{
			FeedBackSleep sleepFeedback{};
			sleepDectectAlgo.MainProcFunc((unsigned char*)bgr24Frame.frameData, sleepFeedback);
			std::vector<AlarmInfo> alarmInfos;
			AlarmInfo alarmInfo{};

			for (int i = 0; i != sleepFeedback.vecShowInfo.size(); ++i)
			{
				alarmInfo.type = SLEEP;
				alarmInfo.x = sleepFeedback.vecShowInfo[i].rRect.x;
				alarmInfo.y = sleepFeedback.vecShowInfo[i].rRect.y;
				alarmInfo.w = sleepFeedback.vecShowInfo[i].rRect.width;
				alarmInfo.h = sleepFeedback.vecShowInfo[i].rRect.height;
				alarmInfo.label = sleepFeedback.vecShowInfo[i].nLabel;
				alarmInfos.push_back(alarmInfo);
			}

			if (0 < alarmInfos.size())
			{
				AlarmMessage message;
				message.setMessageData(
					0, 1920, 1080, "Alarm", alarmInfos, bgr24Frame.jpegData, bgr24Frame.jpegBytes);
				publisherModelPtr->send(message.getMessageData(), message.getMessageBytes());
				LOG(INFO) << "Send SLEEP alarm " << ++sleepMessageNumber;
			}

			bgr24FrameCaches[SLEEP].pop_front();
		}
	}
}

static void phoneDectctAlgo(const int w = 1920, const int h = 1080, const int channel = 3)
{
	while (1)
	{
		BGR24Frame bgr24Frame;
		bool status{ bgr24FrameCaches[PHONE].front(bgr24Frame) };

		if (status)
		{
			FeedBackPhone phoneFeedback{};
			phoneDectectAlgo.MainProcFunc((unsigned char*)bgr24Frame.frameData, phoneFeedback);
			std::vector<AlarmInfo> alarmInfos;
			AlarmInfo alarmInfo{};

			for (int i = 0; i != phoneFeedback.vecShowInfo.size(); ++i)
			{
				alarmInfo.type = PHONE;
				alarmInfo.x = phoneFeedback.vecShowInfo[i].rRect.x;
				alarmInfo.y = phoneFeedback.vecShowInfo[i].rRect.y;
				alarmInfo.w = phoneFeedback.vecShowInfo[i].rRect.width;
				alarmInfo.h = phoneFeedback.vecShowInfo[i].rRect.height;
				alarmInfo.label = phoneFeedback.vecShowInfo[i].nLabel;
				alarmInfos.push_back(alarmInfo);
			}

			if (0 < alarmInfos.size())
			{
				AlarmMessage message;
				message.setMessageData(
					0, 1920, 1080, "Alarm", alarmInfos, bgr24Frame.jpegData, bgr24Frame.jpegBytes);
				publisherModelPtr->send(message.getMessageData(), message.getMessageBytes());
				LOG(INFO) << "Send PHONE alarm " << ++phoneMessageNumber;
			}

			bgr24FrameCaches[PHONE].pop_front();
		}
	}
}

static void helmetDectctAlgo(const int w = 1920, const int h = 1080, const int channel = 3)
{
	while (1)
	{
		BGR24Frame bgr24Frame;
		bool status{ bgr24FrameCaches[HELMET].front(bgr24Frame) };

		if (status)
		{
			FeedBackHelmet helmetFeedback{};
			helmetDetectAlgo.MainProcFunc((unsigned char*)bgr24Frame.frameData, helmetFeedback);
			std::vector<AlarmInfo> alarmInfos;
			AlarmInfo alarmInfo{};

			for (int i = 0; i != helmetFeedback.vecShowInfo.size(); ++i)
			{
				alarmInfo.type = HELMET;
				alarmInfo.x = helmetFeedback.vecShowInfo[i].rRect.x;
				alarmInfo.y = helmetFeedback.vecShowInfo[i].rRect.y;
				alarmInfo.w = helmetFeedback.vecShowInfo[i].rRect.width;
				alarmInfo.h = helmetFeedback.vecShowInfo[i].rRect.height;
				alarmInfo.label = helmetFeedback.vecShowInfo[i].nLabel;
				alarmInfos.push_back(alarmInfo);
			}

			if (0 < alarmInfos.size())
			{
				AlarmMessage message;
				message.setMessageData(
					0, 1920, 1080, "Alarm", alarmInfos, bgr24Frame.jpegData, bgr24Frame.jpegBytes);
				publisherModelPtr->send(message.getMessageData(), message.getMessageBytes());
				LOG(INFO) << "Send HELMET alarm " << ++helmetMessageNumber;
			}

			bgr24FrameCaches[HELMET].pop_front();
		}
	}
}

static void initSleepAlgo(const std::string path, const int w = 1920, const int h = 1080, const int channel = 3)
{
	const std::string cfgFile{ (boost::format("%s\\model\\net.cfg") % path).str() };
	const std::string weightFile{ (boost::format("%s\\model\\helmet_sleep.weights") % path).str() };
	StruInitParams ap{};
	ap.detectThreshold = 0.2f;
	ap.trackThreshold = 0.15f;
	ap.cfgfile = (char*)cfgFile.c_str();
	ap.weightFile = (char*)weightFile.c_str();
	ap.savePath = (char*)"";

	const bool status{ sleepDectectAlgo.InitAlgoriParam(w, h, channel, ap) };
	if (!status)
	{
	 	LOG(ERROR) << "Initialize algo [ Sleep ] failed : " << ap.cfgfile << " : " << ap.weightFile << ".";
	}
	else
	{
	 	LOG(INFO) << "Initialize algo [ Sleep ] success : " << ap.cfgfile << " : " << ap.weightFile << ".";
	}
}

static void initPhoneAlgo(const std::string path, const int w = 1920, const int h = 1080, const int channel = 3)
{
	const std::string cfgFile{ (boost::format("%s\\model\\net.cfg") % path).str() };
	const std::string weightFile{ (boost::format("%s\\model\\helmet_sleep.weights") % path).str() };
	StruInitParams ap{};
	ap.detectThreshold = 0.9f;
	ap.trackThreshold = 0.2f;
	ap.cfgfile = (char*)cfgFile.c_str();
	ap.weightFile = (char*)weightFile.c_str();
	ap.savePath = (char*)"";

	const bool status{ phoneDectectAlgo.InitAlgoriParam(w, h, channel, ap) };
	if (!status)
	{
	 	LOG(ERROR) << "Initialize algo [ Phone ] failed : " << ap.cfgfile << " : " << ap.weightFile << ".";
	}
	else
	{
	 	LOG(INFO) << "Initialize algo [ Phone ] success : " << ap.cfgfile << " : " << ap.weightFile << ".";
	}
}

static void initHelmetAlgo(const std::string path, const int w = 1920, const int h = 1080, const int channel = 3)
{
	const std::string cfgFile{ (boost::format("%s\\model\\net.cfg") % path).str() };
	const std::string weightFile{ (boost::format("%s\\model\\helmet_sleep.weights") % path).str() };
	StruInitParams ap{};
	ap.detectThreshold = 0.25f;
	ap.trackThreshold = 0.15f;
	ap.cfgfile = (char*)cfgFile.c_str();
	ap.weightFile = (char*)weightFile.c_str();
	ap.savePath = (char*)"";

	const bool status{ helmetDetectAlgo.InitAlgoriParam(w, h, channel, ap) };
	if (!status)
	{
	 	LOG(ERROR) << "Initialize algo [ Helmet ] failed : " << ap.cfgfile << " : " << ap.weightFile << ".";
	}
	else
	{
	 	LOG(INFO) << "Initialize algo [ Helmet ] success : " << ap.cfgfile << " : " << ap.weightFile << ".";
	}
}

static unsigned int __stdcall algoWorkerThreadFunc(void* ctx = NULL)
{
	AIAlgoContext* algoctx{ reinterpret_cast<AIAlgoContext*>(ctx) };

	if (HELMET == algoctx->algo)
	{
		helmetDectctAlgo();
	}
	else if (PHONE == algoctx->algo)
	{
		phoneDectctAlgo();
	}
	else if (SLEEP == algoctx->algo)
	{
		sleepDectctAlgo();
	}
	else if (FIGHT == algoctx->algo)
	{
	}
	else if (OFF_DUTY == algoctx->algo)
	{
	}

	return 0;
}

static void initAlgo(void)
{
	const std::string exePath{ boost::filesystem::initial_path<boost::filesystem::path>().string() };
	initHelmetAlgo(exePath);
	initPhoneAlgo(exePath);
	initSleepAlgo(exePath);
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

	initAlgo();

	try
	{
		PTREE rootNode;
		boost::property_tree::ini_parser::read_ini("config.ini", rootNode);

		int publisherPortNumber{ rootNode.get_child("Publisher").get<int>("Port") }, 
			routerPortNumber{ rootNode.get_child("Router").get<int>("Port") }, 
			cpuCoreNumber{ CPU().getCPUCoreNumber() };
		boost::shared_ptr<MQModel> publisherPtr{ boost::make_shared<PublisherModel>(publisherPortNumber) };
		boost::shared_ptr<MQModel> routerPtr{ boost::make_shared<DeviceHostServer>(routerPortNumber, bgr24FrameCaches) };

		if (publisherPtr && routerPtr)
		{
			publisherModelPtr.swap(publisherPtr);
			routerModelPtr.swap(routerPtr);

			if (ERR_OK == publisherModelPtr->start(cpuCoreNumber) && ERR_OK == routerModelPtr->start(cpuCoreNumber))
			{
				LOG(INFO) << "Start publisher port number " << publisherPortNumber << 
					" and router port number " << routerPortNumber << 
					" and CPU core number " << cpuCoreNumber << ".";

				for (int i = HELMET; i != ALGO_NONE; ++i)
				{
					algoContext[i].algo = (AIAlgo)i;
					HANDLE handle = (HANDLE)_beginthreadex(NULL, 0, &algoWorkerThreadFunc, &algoContext[i], 0, NULL);
					SetThreadPriority(handle, THREAD_PRIORITY_TIME_CRITICAL);
					const int maskValue{ 1 << i };
					SetThreadAffinityMask(handle, maskValue);
				}
			}
		}
		else
		{
			LOG(ERROR) << "Failed to create publisher and router service.";
		}

		getchar();

		if (publisherModelPtr)
		{
			publisherModelPtr->stop();
			publisherModelPtr.reset();
		}

		if (routerModelPtr)
		{
			routerModelPtr->stop();
			routerModelPtr.reset();
		}
	}
	catch (std::exception& e)
	{
		LOG(ERROR) << e.what();
	}

	google::ShutdownGoogleLogging();
	return 0;
}
