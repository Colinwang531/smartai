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
	boost::winapi::ULONGLONG_ lastKnownTickTime{ 0 };
//	boost::winapi::ULONGLONG_ runtimeForPerLivestream{ GetTickCount64() };

	while (1)
	{
		livestreamMtx.lock();
		LivestreamGroup livestreams{ livestreamGroup };
		livestreamMtx.unlock();

		for (boost::unordered_map<const std::string, LivestreamPtr>::iterator it = livestreams.begin(); it != livestreams.end(); it++)
		{
			helmet.clear_oldvec();
			std::vector<void*> bgr24FrameQueue;
			it->second->queue(ALGO_HELMET, bgr24FrameQueue);
			FeedBackHelmet feedback;

			for (std::vector<void*>::iterator it = bgr24FrameQueue.begin(); it != bgr24FrameQueue.end();)
			{
				BGR24Frame* frame{ reinterpret_cast<BGR24Frame*>(*it) };
				boost::winapi::ULONGLONG_ mainProcTime{ GetTickCount64() };
				bool result{ helmet.MainProcFunc((unsigned char*)frame->frameData, feedback) };
				printf("=====  MainProcFunc run time = %lld.\r\n", GetTickCount64() - mainProcTime);

				if (result)
				{
					DetectNotify detectNotify;
					std::vector<DetectNotify> detectNotifies;
					std::map<int, StruMemoryInfo>::iterator iter = feedback.mapMemory.begin();

					for (; iter != feedback.mapMemory.end(); ++iter)
					{
						float maxConfidence = 0.0f;
						int nSaveId = 0;
						for (int i = 0; i < iter->second.vecSaveMat.size(); i++)
						{
							if (iter->second.vecSaveMat[i].detectConfidence > maxConfidence)
							{
								maxConfidence = iter->second.vecSaveMat[i].detectConfidence;
								nSaveId = i;
							}
						}

						detectNotify.type = ALGO_HELMET;
						detectNotify.x = iter->second.vecSaveMat[nSaveId].rRect.x;
						detectNotify.y = iter->second.vecSaveMat[nSaveId].rRect.y;
						detectNotify.w = iter->second.vecSaveMat[nSaveId].rRect.width;
						detectNotify.h = iter->second.vecSaveMat[nSaveId].rRect.height;
						detectNotify.status = iter->second.vecSaveMat[nSaveId].nLabel;
						detectNotifies.push_back(detectNotify);

						if (0 < detectNotifies.size() && captureAlarmNotifyHandler)
						{
							boost::winapi::ULONGLONG_ currentTickTime{ GetTickCount64() };

							if (!lastKnownTickTime || 5000 < currentTickTime - lastKnownTickTime)
							{
								lastKnownTickTime = currentTickTime;
								captureAlarmNotifyHandler(frame, detectNotifies);
							}
						}
					}
				}

				boost::checked_array_delete(frame->frameData);
				boost::checked_array_delete(frame->jpegData);
				boost::checked_array_delete(frame->NVRIp);
				boost::checked_delete(frame);
				it = bgr24FrameQueue.erase(it);
				feedback.vecShowInfo.clear();
				feedback.mapMemory.clear();
			}

// 			boost::winapi::ULONGLONG_ now{ GetTickCount64() };
// 			printf("=====  Process run time = %lld.\r\n", now - runtimeForPerLivestream);
// 			runtimeForPerLivestream = now;
		}
	}
}

NS_END