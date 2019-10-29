#include "boost/format.hpp"
#include "boost/checked_delete.hpp"
#include "error.h"
#include "Arithmetic/CVAlgoSleep.h"

NS_BEGIN(algo, 1)

CVAlgoSleep::CVAlgoSleep(CaptureAlarmInfoHandler handler /* = NULL */)
	: CVAlgo(handler)
{}

CVAlgoSleep::~CVAlgoSleep()
{}

int CVAlgoSleep::initializeWithParameter(const char* configFilePath /* = NULL */, void* parameter /* = NULL */)
{
	int status{ ERR_OK };
	const std::string cfgFile{ (boost::format("%s\\model\\helmet_sleep.cfg") % configFilePath).str() };
	const std::string weightFile{ (boost::format("%s\\model\\helmet_sleep.weights") % configFilePath).str() };
	StruInitParams* initParames{ reinterpret_cast<StruInitParams*>(parameter) };
	initParames->cfgfile = (char*)cfgFile.c_str();
	initParames->weightFile = (char*)weightFile.c_str();
	initParames->sleepTime = 10;
	if (initParames)
	{
		status = sleep.InitAlgoriParam(
			IMAGE_WIDTH, IMAGE_HEIGHT, CHANNEL_NUMBER, *initParames) ? ERR_OK : ERR_BAD_OPERATE;
	}

	return status;
}

void CVAlgoSleep::arithmeticWorkerProcess()
{
//	boost::winapi::ULONGLONG_ lastKnownTickTime{ 0 };

	while (1)
	{
		MediaImagePtr bgr24ImagePtr{ BGR24ImageQueue.remove() };

		if (bgr24ImagePtr)
		{
			std::vector<AlarmInfo> alarmInfos;
//			boost::winapi::ULONGLONG_ mainProcTime{ GetTickCount64() };
			sleep.MainProcFunc((unsigned char*)bgr24ImagePtr->getImage(), feedback);
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

					bgr24ImagePtr->setOriginImage(
						(const unsigned char*)(it->second.vecSaveMat[nSaveId].pUcharImage), IMAGE_WIDTH * IMAGE_HEIGHT * 3);
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

				if (0 < alarmInfos.size() && captureAlarmInfoHandler)
				{
					boost::winapi::ULONGLONG_ currentTickTime{ GetTickCount64() };

					if (!lastKnownTickTime || 5000 < currentTickTime - lastKnownTickTime)
					{
						lastKnownTickTime = currentTickTime;
						captureAlarmInfoHandler(bgr24ImagePtr, alarmInfos);
					}
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