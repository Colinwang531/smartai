#include "boost/format.hpp"
#include "boost/winapi/time.hpp"
#include "boost/checked_delete.hpp"
#include "error.h"
#include "Arithmetic/CVAlgoHelmet.h"

NS_BEGIN(algo, 1)

CVAlgoHelmet::CVAlgoHelmet(CaptureAlarmNotifyHandler handler /* = NULL */)
	: CVAlgo(handler)
{}

CVAlgoHelmet::~CVAlgoHelmet()
{}

bool CVAlgoHelmet::initializeWithParameter(const char* configFilePath /* = NULL */, void* parameter /* = NULL */)
{
	bool status{ false };
	const std::string cfgFile{ (boost::format("%s\\model\\helmet_sleep.cfg") % configFilePath).str() };
	const std::string weightFile{ (boost::format("%s\\model\\helmet_sleep.weights") % configFilePath).str() };
	StruInitParams* initParames{ reinterpret_cast<StruInitParams*>(parameter) };
	initParames->cfgfile = (char*)cfgFile.c_str();
	initParames->weightFile = (char*)weightFile.c_str();

	if (initParames)
	{
		status = helmet.InitAlgoriParam(IMAGE_WIDTH, IMAGE_HEIGHT, CHANNEL_NUMBER, *initParames) ? ERR_OK : ERR_BAD_OPERATE;
	}

	return status;
}

void CVAlgoHelmet::algorithmWorkerProcess()
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
 				FeedBackHelmet feedback;
// 				unsigned long long lastKnownTime = GetTickCount64();
				bool result{ helmet.MainProcFunc((unsigned char*)frame->frameData, feedback) };
// 				unsigned long long currentTime = GetTickCount64();
// 				printf("[Helmet] MainProcFunc expire %I64u, vecShowInfo size %d.\r\n", currentTime - lastKnownTime, (int)feedback.vecShowInfo.size());

				if (result)
				{
					DetectNotify dectectNotify;
					std::vector<DetectNotify> detectNotifies;

					for (int i = 0; i != feedback.vecShowInfo.size(); ++i)
					{
						dectectNotify.type = ALGO_HELMET;
						dectectNotify.x = feedback.vecShowInfo[i].rRect.x;
						dectectNotify.y = feedback.vecShowInfo[i].rRect.y;
						dectectNotify.w = feedback.vecShowInfo[i].rRect.width;
						dectectNotify.h = feedback.vecShowInfo[i].rRect.height;
						dectectNotify.status = feedback.vecShowInfo[i].nLabel;
						detectNotifies.push_back(dectectNotify);
					}

					if (0 < detectNotifies.size() && captureAlarmNotifyHandler)
					{
						captureAlarmNotifyHandler(frame, detectNotifies);
					}
				}

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