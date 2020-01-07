#include "boost/filesystem.hpp"
#include "boost/format.hpp"
#include "boost/checked_delete.hpp"
#include "error.h"
#include "Arithmetic/CVAlgoSleep.h"

NS_BEGIN(algo, 1)

CVAlgoSleep::CVAlgoSleep() : CVAlgo()
{}

CVAlgoSleep::~CVAlgoSleep()
{}

int CVAlgoSleep::setTimeSleepInterval(const int interval)
{
	StruInitParams parameters;
	parameters.sleepTime = interval;
	return sleep.SetSleepTime(parameters) ? ERR_OK : ERR_BAD_OPERATE;
}

int CVAlgoSleep::initializeArithmetic()
{
	const std::string executePath{
		boost::filesystem::initial_path<boost::filesystem::path>().string() };
	const std::string cfgFile{ (boost::format("%s\\model\\helmet_sleep.cfg") % executePath).str() };
	const std::string weightFile{ (boost::format("%s\\model\\helmet_sleep.weights") % executePath).str() };
	StruInitParams parameters;
	parameters.gpu_id = 0;
	parameters.sleepTime = 15;
	parameters.detectThreshold = 0.65f;
	parameters.trackThreshold = 0.15f;
	parameters.cfgfile = (char*)cfgFile.c_str();
	parameters.weightFile = (char*)weightFile.c_str();

	return sleep.InitAlgoriParam(
		IMAGE_WIDTH, IMAGE_HEIGHT, CHANNEL_NUMBER, parameters) ? ERR_OK : ERR_BAD_OPERATE;
}

int CVAlgoSleep::deinitializeArithmetic()
{
	return ERR_OK;
}

void CVAlgoSleep::arithmeticWorkerProcess()
{
//	boost::winapi::ULONGLONG_ lastKnownTickTime{ 0 };

	while (1)
	{
		MediaDataPtr bgr24ImagePtr{ BGR24ImageQueue.remove() };

		if (bgr24ImagePtr)
		{
			std::vector<AlarmInfo> alarmInfos;
//			boost::winapi::ULONGLONG_ mainProcTime{ GetTickCount64() };
			sleep.MainProcFunc((unsigned char*)bgr24ImagePtr->getData(), feedback);
//			printf("=====  MainProcFunc run time = %lld.\r\n", GetTickCount64() - mainProcTime);

			typedef std::map<int, StruMemoryInfo>::iterator Iterator;
//			printf("_____Total mapMemory size = %d.\r\n", (int)feedback.mapMemory.size());
			for (Iterator it = feedback.mapMemory.begin(); it != feedback.mapMemory.end();)
			{
//				printf("_____Item ncath = %d.", it->second.nCatch);
				if (1 == it->second.nCatch)
				{
					int nSaveId{ (int)(it->second.vecSaveMat.size() - 1) };
					it->second.nCatch = 2;

					AlarmInfo alarmInfo;
					alarmInfo.type = AlarmType::ALARM_TYPE_SLEEP;
					alarmInfo.x = it->second.vecSaveMat[nSaveId].rRect.x;
					alarmInfo.y = it->second.vecSaveMat[nSaveId].rRect.y;
					alarmInfo.w = it->second.vecSaveMat[nSaveId].rRect.width;
					alarmInfo.h = it->second.vecSaveMat[nSaveId].rRect.height;
					alarmInfo.status = it->second.vecSaveMat[nSaveId].nLabel;
					alarmInfos.push_back(alarmInfo);
//					printf("=====  vecShowInfo.detectCOnfidence = %f.\r\n", feedback.vecShowInfo);

// 					bgr24ImagePtr->setOriginImage(
// 						(const unsigned char*)(it->second.vecSaveMat[nSaveId].pUcharImage), IMAGE_WIDTH * IMAGE_HEIGHT * 3);
					boost::checked_array_delete(it->second.vecSaveMat[nSaveId].pUcharImage);
//					it = feedback.mapMemory.erase(it);
				}
				//else
				//{
				//	++it;
				//}

				if(it->second.bDone)
				{
					it = feedback.mapMemory.erase(it);
				}
				else
				{
					it++;
				}

// 				if (0 < alarmInfos.size())
// 				{
// //					boost::winapi::ULONGLONG_ currentTickTime{ GetTickCount64() };
// 
// // 					if (!lastKnownTickTime || 5000 < currentTickTime - lastKnownTickTime)
// // 					{
// // 						lastKnownTickTime = currentTickTime;
// // 						captureAlarmInfoHandler(bgr24ImagePtr, alarmInfos);
// // 					}
// 				}
				if (0 < alarmInfos.size() && postDetectAlarmInfoCallback)
				{
					postDetectAlarmInfoCallback(alarmInfos, bgr24ImagePtr->getData(), bgr24ImagePtr->getDataBytes());
				}
			}
		}
		else
		{
			break;
		}
	}
}

NS_END