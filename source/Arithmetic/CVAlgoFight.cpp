#include "boost/format.hpp"
#include "boost/winapi/time.hpp"
#include "boost/checked_delete.hpp"
#include "error.h"
#include "Arithmetic/CVAlgoFight.h"

NS_BEGIN(algo, 1)

CVAlgoFight::CVAlgoFight(CaptureAlarmNotifyHandler handler /* = NULL */)
	: CVAlgo(handler)
{}

CVAlgoFight::~CVAlgoFight()
{}

bool CVAlgoFight::initializeWithParameter(const char* configFilePath /* = NULL */, void* parameter /* = NULL */)
{
	bool status{ false };
	const std::string cfgFile{ (boost::format("%s\\model\\fight.cfg") % configFilePath).str() };
	const std::string weightFile{ (boost::format("%s\\model\\fight.weights") % configFilePath).str() };
	StruInitParams* initParames{ reinterpret_cast<StruInitParams*>(parameter) };
	initParames->cfgfile = (char*)cfgFile.c_str();
	initParames->weightFile = (char*)weightFile.c_str();

	if (initParames)
	{
		status = fight.InitAlgoriParam(IMAGE_WIDTH, IMAGE_HEIGHT, CHANNEL_NUMBER, *initParames) ? ERR_OK : ERR_BAD_OPERATE;
	}

	return status;
}

void CVAlgoFight::algorithmWorkerProcess()
{
	while (1)
	{
		for (boost::unordered_map<const std::string, LivestreamPtr>::iterator it = livestreamGroup.begin(); it != livestreamGroup.end(); it++)
		{
			std::vector<void*> bgr24FrameQueue;
			it->second->queue(ALGO_HELMET, bgr24FrameQueue);

			for (std::vector<void*>::iterator it = bgr24FrameQueue.begin(); it != bgr24FrameQueue.end();)
			{
				BGR24Frame* frame{ reinterpret_cast<BGR24Frame*>(*it) };
				FeedBackFight feedback;
				//			unsigned long long lastKnownTime = GetTickCount64();
// 				bool result{ fight.MainProcFunc((unsigned char*)frame->frameData, feedback) };
// 				//			unsigned long long currentTime = GetTickCount64();
// 				//			printf("[Fight] MainProcFunc expire %I64u, vecShowInfo size %d.\r\n", currentTime - lastKnownTime, (int)feedback.vecShowInfo.size());
// 
// 				if (result)
// 				{
// 					DetectNotify detectNotify;
// 					std::vector<DetectNotify> detectNotifies;
// 
// 					for (int i = 0; i != feedback.vecShowInfo.size(); ++i)
// 					{
// 						detectNotify.type = ALGO_FIGHT;
// 						detectNotify.x = feedback.vecShowInfo[i].rRect.x;
// 						detectNotify.y = feedback.vecShowInfo[i].rRect.y;
// 						detectNotify.w = feedback.vecShowInfo[i].rRect.width;
// 						detectNotify.h = feedback.vecShowInfo[i].rRect.height;
// 						detectNotify.status = feedback.vecShowInfo[i].nLabel;
// 						detectNotifies.push_back(detectNotify);
// 					}
// 
// 					if (0 < detectNotifies.size() && captureAlarmNotifyHandler)
// 					{
// 						captureAlarmNotifyHandler(frame, detectNotifies);
// 					}
// 				}

				boost::checked_array_delete(frame->frameData);
				boost::checked_array_delete(frame->jpegData);
				boost::checked_array_delete(frame->NVRIp);
				boost::checked_delete(frame);
				it = bgr24FrameQueue.erase(it);
			}
		}
	}
}

NS_END