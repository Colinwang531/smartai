#include "boost/format.hpp"
#include "boost/winapi/time.hpp"
#include "boost/checked_delete.hpp"
#include "error.h"
#include "Arithmetic/CVAlgoSleep.h"

NS_BEGIN(algo, 1)

CVAlgoSleep::CVAlgoSleep(CaptureAlarmNotifyHandler handler /* = NULL */)
	: CVAlgo(handler)
{}

CVAlgoSleep::~CVAlgoSleep()
{}

bool CVAlgoSleep::initializeWithParameter(const char* configFilePath /* = NULL */, void* parameter /* = NULL */)
{
	bool status{ false };
	const std::string cfgFile{ (boost::format("%s\\model\\helmet_sleep.cfg") % configFilePath).str() };
	const std::string weightFile{ (boost::format("%s\\model\\helmet_sleep.weights") % configFilePath).str() };
	StruInitParams* initParames{ reinterpret_cast<StruInitParams*>(parameter) };
	initParames->cfgfile = (char*)cfgFile.c_str();
	initParames->weightFile = (char*)weightFile.c_str();
	initParames->sleepTime = 3;

	if (initParames)
	{
		status = sleep.InitAlgoriParam(IMAGE_WIDTH, IMAGE_HEIGHT, CHANNEL_NUMBER, *initParames);
	}

	return status;
}

void CVAlgoSleep::algorithmWorkerProcess()
{
	boost::winapi::ULONGLONG_ lastTickCount = 0;
	bool enableAlarmFlag = false;

	while (1)
	{
		livestreamMtx.lock();
		LivestreamGroup livestreams{ livestreamGroup };
		livestreamMtx.unlock();

		for (boost::unordered_map<const std::string, LivestreamPtr>::iterator it = livestreams.begin(); it != livestreams.end(); it++)
		{
			std::vector<void*> bgr24FrameQueue;
			it->second->queue(ALGO_SLEEP, bgr24FrameQueue);

			for (std::vector<void*>::iterator it = bgr24FrameQueue.begin(); it != bgr24FrameQueue.end();)
			{
				BGR24Frame* frame{ reinterpret_cast<BGR24Frame*>(*it) };
				FeedBackSleep feedback;
				bool result{ sleep.MainProcFunc((unsigned char*)frame->frameData, feedback) };

				if (result)
				{
					DetectNotify detectNotify;
					std::vector<DetectNotify> detectNotifies;

					for (int i = 0; i != feedback.vecShowInfo.size(); ++i)
					{
						if (feedback.vecShowInfo[i].catchTime >= 3)
						{
							detectNotify.type = ALGO_SLEEP;
							detectNotify.x = feedback.vecShowInfo[i].rRect.x;
							detectNotify.y = feedback.vecShowInfo[i].rRect.y;
							detectNotify.w = feedback.vecShowInfo[i].rRect.width;
							detectNotify.h = feedback.vecShowInfo[i].rRect.height;
							detectNotify.status = feedback.vecShowInfo[i].nLabel;
							detectNotifies.push_back(detectNotify);
						}
					}

					boost::winapi::ULONGLONG_ currentTickCount = GetTickCount64();
					if (0 == lastTickCount || currentTickCount - lastTickCount > 30000)
					{
						lastTickCount = currentTickCount;
						enableAlarmFlag = true;
					}
					else
					{
						enableAlarmFlag = false;
					}

					if (enableAlarmFlag && 0 < detectNotifies.size() && captureAlarmNotifyHandler)
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