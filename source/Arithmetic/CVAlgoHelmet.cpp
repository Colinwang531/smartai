#include "boost/format.hpp"
#include "boost/winapi/time.hpp"
#include "boost/checked_delete.hpp"
#include "error.h"
#include "Arithmetic/CVAlgoHelmet.h"

NS_BEGIN(algo, 1)

CVAlgoHelmet::CVAlgoHelmet(const std::string NVRIP, const int cameraIndex, CaptureAlarmInfoHandler handler /* = NULL */)
	: CVAlgo(handler), NVRIpAddress{ NVRIP }, cameraIndexID{ cameraIndex }
{}

CVAlgoHelmet::~CVAlgoHelmet()
{}

int CVAlgoHelmet::initializeWithParameter(const char* configFilePath /* = NULL */, void* parameter /* = NULL */)
{
	int status{ ERR_OK };
	const std::string cfgFile{ (boost::format("%s\\model\\helmet_sleep.cfg") % configFilePath).str() };
	const std::string weightFile{ (boost::format("%s\\model\\helmet_sleep.weights") % configFilePath).str() };
	StruInitParams* initParames{ reinterpret_cast<StruInitParams*>(parameter) };
	initParames->cfgfile = (char*)cfgFile.c_str();
	initParames->weightFile = (char*)weightFile.c_str();

	if (initParames)
	{
		status = helmet.InitAlgoriParam(
			IMAGE_WIDTH, IMAGE_HEIGHT, CHANNEL_NUMBER, *initParames) ? ERR_OK : ERR_BAD_OPERATE;
	}

	return status;
}

void CVAlgoHelmet::arithmeticWorkerProcess()
{
	FeedBackHelmet feedback;
	DWORD threadID{ GetCurrentThreadId() };

	while (1)
	{
		MediaImagePtr bgr24ImagePtr{ BGR24ImageQueue.remove() };

		if (bgr24ImagePtr)
		{
			boost::winapi::ULONGLONG_ mainProcTime{ GetTickCount64() };
			bool result{ helmet.MainProcFunc((unsigned char*)bgr24ImagePtr->getImage(), feedback) };
			printf("=====  MainProcFunc run time = %lld, thread ID = %d, %s_%d.\r\n", 
				GetTickCount64() - mainProcTime, (int)threadID, NVRIpAddress.c_str(), cameraIndexID);

			if (result)
			{
				std::vector<AlarmInfo> alarmInfos;
				for (std::map<int, StruMemoryInfo>::iterator it = feedback.mapMemory.begin(); 
					it != feedback.mapMemory.end(); 
					++it)
				{
					float maxConfidence = 0.0f;
					int nSaveId{ -1 };
					for (int i = 0; i < it->second.vecSaveMat.size(); i++)
					{
						if (it->second.vecSaveMat[i].detectConfidence > maxConfidence)
						{
							maxConfidence = it->second.vecSaveMat[i].detectConfidence;
							nSaveId = i;
						}

						boost::checked_array_delete(it->second.vecSaveMat[i].pUcharImage);
					}

					if (-1 < nSaveId)
					{
						AlarmInfo alarmInfo;
						alarmInfo.type = AlarmType::ALARM_TYPE_HELMET;
						alarmInfo.x = it->second.vecSaveMat[nSaveId].rRect.x;
						alarmInfo.y = it->second.vecSaveMat[nSaveId].rRect.y;
						alarmInfo.w = it->second.vecSaveMat[nSaveId].rRect.width;
						alarmInfo.h = it->second.vecSaveMat[nSaveId].rRect.height;
						alarmInfo.status = it->second.vecSaveMat[nSaveId].nLabel;
						alarmInfos.push_back(alarmInfo);
					}

					if (0 < alarmInfos.size() && captureAlarmInfoHandler)
					{
						captureAlarmInfoHandler(bgr24ImagePtr, alarmInfos);
					}
				}
			}

			feedback.vecShowInfo.clear();
			feedback.mapMemory.clear();
		}
		else
		{
			break;
		}
	}
}

NS_END