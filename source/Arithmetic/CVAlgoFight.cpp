#include "boost/filesystem.hpp"
#include "boost/format.hpp"
#include "boost/winapi/time.hpp"
#include "boost/checked_delete.hpp"
#include "error.h"
#include "Arithmetic/CVAlgoFight.h"

namespace framework
{
	namespace arithmetic
	{
		CVAlgoFight::CVAlgoFight() : CVAlgo()
		{}

		CVAlgoFight::~CVAlgoFight()
		{}

		int CVAlgoFight::initializeArithmetic()
		{
			const std::string executePath{
					boost::filesystem::initial_path<boost::filesystem::path>().string() };
			const std::string cfgFile{ (boost::format("%s\\model\\fight.cfg") % executePath).str() };
			const std::string weightFile{ (boost::format("%s\\model\\fight.weights") % executePath).str() };
			StruInitParams parameters;
			parameters.gpu_id = 0;
			parameters.detectThreshold = 0.99f;
			parameters.trackThreshold = 0.10f;
			parameters.cfgfile = (char*)cfgFile.c_str();
			parameters.weightFile = (char*)weightFile.c_str();

			return fight.InitAlgoriParam(
				IMAGE_WIDTH, IMAGE_HEIGHT, CHANNEL_NUMBER, parameters) ? ERR_OK : ERR_BAD_OPERATE;
		}

		int CVAlgoFight::deinitializeArithmetic()
		{
			return ERR_OK;
		}

		void CVAlgoFight::arithmeticWorkerProcess()
		{
			//	boost::winapi::ULONGLONG_ lastKnownTickTime{ 0 };

			while (1)
			{
				MediaDataPtr bgr24ImagePtr{ BGR24ImageQueue.remove() };

				if (bgr24ImagePtr)
				{
					std::vector<AlarmInfo> alarmInfos;
					//			boost::winapi::ULONGLONG_ mainProcTime{ GetTickCount64() };
					fight.MainProcFunc((unsigned char*)bgr24ImagePtr->getData(), feedback);
					//			printf("=====  MainProcFunc run time = %lld.\r\n", GetTickCount64() - mainProcTime);

					//			printf("=====  feedback.mapMemory.size() = %d.\r\n", feedback.mapMemory.size());

					typedef std::map<int, StruMemoryInfo>::iterator Iterator;
					for (Iterator it = feedback.mapMemory.begin(); it != feedback.mapMemory.end();)
					{
						// 				if (it->second.bDone)
						// 				{
						int validNumber{ 0 };
						float maxConfidence{ 0.0f };
						int nSaveId{ (int)(it->second.vecSaveMat.size() - 1) };

						for (int i = 0; i < it->second.vecSaveMat.size(); i++)
						{
							//						printf("=====  detectConfidence = %f.\r\n", it->second.vecSaveMat[i].detectConfidence);
							if (0.997f < it->second.vecSaveMat[i].detectConfidence)
							{
								++validNumber;
							}

							// 						if (it->second.vecSaveMat[i].detectConfidence > maxConfidence)
							// 						{
							// 							maxConfidence = it->second.vecSaveMat[i].detectConfidence;
							// 							nSaveId = i;
							// 						}
						}

						if (0 < validNumber && -1 < nSaveId)
						{
							AlarmInfo alarmInfo;
							alarmInfo.type = AlarmType::ALARM_TYPE_FIGHT;
							alarmInfo.x = it->second.vecSaveMat[nSaveId].rRect.x;
							alarmInfo.y = it->second.vecSaveMat[nSaveId].rRect.y;
							alarmInfo.w = it->second.vecSaveMat[nSaveId].rRect.width;
							alarmInfo.h = it->second.vecSaveMat[nSaveId].rRect.height;
							alarmInfo.status = it->second.vecSaveMat[nSaveId].nLabel;
							alarmInfos.push_back(alarmInfo);

							// 						bgr24ImagePtr->setOriginImage(
							// 							(const unsigned char*)(it->second.vecSaveMat[nSaveId].pUcharImage), IMAGE_WIDTH * IMAGE_HEIGHT * 3);
							boost::checked_array_delete(it->second.vecSaveMat[nSaveId].pUcharImage);
							it = feedback.mapMemory.erase(it);
						}
						else
						{
							++it;
						}

						// 					if (0 < alarmInfos.size())
						// 					{
						// //						boost::winapi::ULONGLONG_ currentTickTime{ GetTickCount64() };
						// 
						// // 						if (!lastKnownTickTime || 3000 < currentTickTime - lastKnownTickTime)
						// // 						{
						// // 							lastKnownTickTime = currentTickTime;
						// // 							captureAlarmInfoHandler(bgr24ImagePtr, alarmInfos);
						// // 						}
						// 					}

						if (0 < alarmInfos.size() && postDetectAlarmInfoCallback)
						{
							postDetectAlarmInfoCallback(alarmInfos[0], (unsigned char*)bgr24ImagePtr->getData(), (int)bgr24ImagePtr->getDataBytes());
						}
						// 				}
						// 				else
						// 				{
						// 					++it;
						// 				}
					}
				}
				else
				{
					break;
				}
			}
		}
	}//namespace arithmetic
}//namespace framework
