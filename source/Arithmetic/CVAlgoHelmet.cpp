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
	std::vector<StruResult> lastKnownMotionRect;

	while (1)
	{
		livestreamMtx.lock();
		LivestreamGroup livestreams{ livestreamGroup };
		livestreamMtx.unlock();

		for (boost::unordered_map<const std::string, LivestreamPtr>::iterator it = livestreams.begin(); it != livestreams.end(); it++)
		{
			std::vector<StruResult> currentMotionRect;
			std::vector<void*> bgr24FrameQueue;
			it->second->queue(ALGO_HELMET, bgr24FrameQueue);
//			bool isLastFrame{ true };

			for (std::vector<void*>::iterator it = bgr24FrameQueue.begin(); it != bgr24FrameQueue.end();)
			{
				BGR24Frame* frame{ reinterpret_cast<BGR24Frame*>(*it) };
 				FeedBackHelmet feedback;
				bool result{ helmet.MainProcFunc((unsigned char*)frame->frameData, feedback) };

				if (result)
				{
//					result = helmet.PostProcessFunc(feedback);
// 					if (!lastKnownMotionRect.size() && 0 < feedback.vecShowInfo.size())
// 					{
// 						lastKnownMotionRect.push_back(feedback.vecShowInfo[0]);
// 						currentMotionRect.push_back(feedback.vecShowInfo[0]);
// 					}

// 					if (it == bgr24FrameQueue.end() - 1)
// 					{
// 						if (0 < lastKnownMotionRect.size())
// 						{
// 							for (int i = 0; i != feedback.vecShowInfo.size(); ++i)
// 							{
// 								bool foundSamePos{ false };
// 
// 								for (int j = 0; j != lastKnownMotionRect.size(); ++j)
// 								{
// 									const int diff_x{ abs(lastKnownMotionRect[j].rRect.x - feedback.vecShowInfo[i].rRect.x) };
// 									const int diff_y{ abs(lastKnownMotionRect[j].rRect.y - feedback.vecShowInfo[i].rRect.y) };
// 									const int diff_w{ abs(lastKnownMotionRect[j].rRect.width - feedback.vecShowInfo[i].rRect.width) };
// 									const int diff_h{ abs(lastKnownMotionRect[j].rRect.height - feedback.vecShowInfo[i].rRect.height) };
// 
// 									if ((50 > diff_x && 50 > diff_y)/* || (50 > diff_w && 50 > diff_h)*/)
// 									{
// 										printf_s("dx = %d, dy = %d, dw =  %d, dh = %d.\r\n", diff_x, diff_y, diff_w, diff_h);
// 										foundSamePos = true;
// 									}
// 								}
// 
// 								if (!foundSamePos)
// 								{
// 									currentMotionRect.push_back(feedback.vecShowInfo[i]);
// 								}
// 
// 								printf_s("Current motion rect count %d, last motion rect %d, vectShowInfo rect %d.\r\n",
// 									currentMotionRect.size(), lastKnownMotionRect.size(), feedback.vecShowInfo.size());
// 							}
// 						}
// 						else
// 						{
// 							currentMotionRect.clear();
// 							currentMotionRect = feedback.vecShowInfo;
// 						}
// 
// //						isFirstFrame = false;
// 						DetectNotify dectectNotify;
// 						std::vector<DetectNotify> detectNotifies;
// 						for (int i = 0; i != currentMotionRect.size(); ++i)
// 						{
// 							dectectNotify.type = ALGO_HELMET;
// 							dectectNotify.x = currentMotionRect[i].rRect.x;
// 							dectectNotify.y = currentMotionRect[i].rRect.y;
// 							dectectNotify.w = currentMotionRect[i].rRect.width;
// 							dectectNotify.h = currentMotionRect[i].rRect.height;
// 							dectectNotify.status = currentMotionRect[i].nLabel;
// 							detectNotifies.push_back(dectectNotify);
// 						}
//
					
					DetectNotify dectectNotify;
					std::vector<DetectNotify> detectNotifies;
					map<int, StruMemoryInfo>::iterator iter = feedback.mapMemory.begin();

					for (; iter != feedback.mapMemory.end(); ++iter)
					{
// 						if (!iter->second.bDone)
// 						{
// 							continue;
// 						}
						// 计算最大的detectConfidence 和 fTrackConfidence;
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

						dectectNotify.type = ALGO_HELMET;
						dectectNotify.x = iter->second.vecSaveMat[nSaveId].rRect.x;
						dectectNotify.y = iter->second.vecSaveMat[nSaveId].rRect.y;
						dectectNotify.w = iter->second.vecSaveMat[nSaveId].rRect.width;
						dectectNotify.h = iter->second.vecSaveMat[nSaveId].rRect.height;
						dectectNotify.status = iter->second.vecSaveMat[nSaveId].nLabel;
						detectNotifies.push_back(dectectNotify);

						captureAlarmNotifyHandler(frame, detectNotifies);

// 						if (0 < detectNotifies.size() && captureAlarmNotifyHandler)
// 						{
// 							boost::winapi::ULONGLONG_ currentTickTime{ GetTickCount64() };
// 
// 							if (!lastKnownTickTime || 5000 < currentTickTime - lastKnownTickTime)
// 							{
// 								lastKnownTickTime = currentTickTime;
// 								captureAlarmNotifyHandler(frame, detectNotifies);
// 							}
// 						}
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

			lastKnownMotionRect.swap(currentMotionRect);
			break;
		}
	}
}

NS_END