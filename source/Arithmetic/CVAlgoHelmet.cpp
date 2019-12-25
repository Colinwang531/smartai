#include "boost/filesystem.hpp"
#include "boost/format.hpp"
#include "boost/checked_delete.hpp"
#include "error.h"
#include "Arithmetic/CVAlgoHelmet.h"

NS_BEGIN(algo, 1)

CVAlgoHelmet::CVAlgoHelmet() : CVAlgo()
{}

CVAlgoHelmet::~CVAlgoHelmet()
{}

int CVAlgoHelmet::initializeArithmetic()
{
	const std::string executePath{
				boost::filesystem::initial_path<boost::filesystem::path>().string() };
	const std::string cfgFile{ (boost::format("%s\\model\\helmet_sleep.cfg") % executePath).str() };
	const std::string weightFile{ (boost::format("%s\\model\\helmet_sleep.weights") % executePath).str() };
	StruInitParams parameters;
	parameters.gpu_id = 0;
	parameters.detectThreshold = 0.20f;
	parameters.trackThreshold = 0.15f;
	parameters.cfgfile = (char*)cfgFile.c_str();
	parameters.weightFile = (char*)weightFile.c_str();

	return helmet.InitAlgoriParam(
		IMAGE_WIDTH, IMAGE_HEIGHT, CHANNEL_NUMBER, parameters) ? ERR_OK : ERR_BAD_OPERATE;
}

int CVAlgoHelmet::deinitializeArithmetic(void)
{
	return ERR_OK;
}

void CVAlgoHelmet::arithmeticWorkerProcess()
{
	while (1)
	{
		MediaDataPtr bgr24ImagePtr{ BGR24ImageQueue.remove() };

		if (bgr24ImagePtr)
		{
			std::vector<AlarmInfo> alarmInfos;
			helmet.MainProcFunc((unsigned char*)bgr24ImagePtr->getData(), feedback);

			typedef std::map<int, StruMemoryInfo>::iterator Iterator;
			for (Iterator it = feedback.mapMemory.begin(); it != feedback.mapMemory.end();)
			{
// 				if (it->second.bDone)
// 				{
					int noneHelmetNumber{ 0 }, haveHelmetNumber{ 0 };
					float maxConfidence{ 0.0f };
					int nSaveId{ (int)(it->second.vecSaveMat.size() - 1) };

					for (int i = 0; i != it->second.vecSaveMat.size(); i++)
					{
						if (0 == it->second.vecSaveMat[i].nLabel)
						{
							noneHelmetNumber++;
						}
						else
						{
							haveHelmetNumber++;
						}

// 						if (it->second.vecSaveMat[i].detectConfidence > maxConfidence)
// 						{
// 							maxConfidence = it->second.vecSaveMat[i].detectConfidence;
// 							nSaveId = i;
// 						}
					}

					if (/*noneHelmetNumber > haveHelmetNumber &&*/ -1 < nSaveId)
					{
						AlarmInfo alarmInfo;
						alarmInfo.type = AlarmType::ALARM_TYPE_HELMET;
						alarmInfo.x = it->second.vecSaveMat[nSaveId].rRect.x;
						alarmInfo.y = it->second.vecSaveMat[nSaveId].rRect.y;
						alarmInfo.w = it->second.vecSaveMat[nSaveId].rRect.width;
						alarmInfo.h = it->second.vecSaveMat[nSaveId].rRect.height;
						alarmInfo.status = noneHelmetNumber > haveHelmetNumber ? 0 : 1;// it->second.vecSaveMat[nSaveId].nLabel;
						alarmInfos.push_back(alarmInfo);

						boost::checked_array_delete(it->second.vecSaveMat[nSaveId].pUcharImage);
						it = feedback.mapMemory.erase(it);
					}
					else
					{
						++it;
					}
// 				}
// 				else
// 				{
// 					++it;
// 				}
			}

			if (0 < alarmInfos.size() && postDetectAlarmInfoCallback)
			{
				postDetectAlarmInfoCallback(alarmInfos, bgr24ImagePtr->getData(), bgr24ImagePtr->getDataBytes());
			}
		}
		else
		{
			break;
		}
	}
}

NS_END