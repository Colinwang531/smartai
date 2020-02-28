#include "boost/format.hpp"
#include "boost/winapi/time.hpp"
#include "boost/checked_delete.hpp"
#include "opencv2/opencv.hpp"
#include "error.h"
#include "Arithmetic/CVAlgoPhone.h"

NS_BEGIN(algo, 1)

CVAlgoPhone::CVAlgoPhone(CaptureAlarmInfoHandler handler /* = NULL */)
	: CVAlgo(handler)
{}

CVAlgoPhone::~CVAlgoPhone()
{}

int CVAlgoPhone::initializeWithParameter(const char* configFilePath /* = NULL */, void* parameter /* = NULL */)
{
	bool status{ ERR_OK };
	const std::string cfgFile{ (boost::format("%s\\model\\phone.cfg") % configFilePath).str() };
	const std::string weightFile{ (boost::format("%s\\model\\phone.weights") % configFilePath).str() };
	StruInitParams* initParames{ reinterpret_cast<StruInitParams*>(parameter) };
	initParames->cfgfile = (char*)cfgFile.c_str();
	initParames->weightFile = (char*)weightFile.c_str();

	const std::string weightFile2{ (boost::format("%s\\model\\phonesecond.weights") % configFilePath).str() };
	StruInitParams ap_second;
	ap_second.gpu_id = initParames->gpu_id;
	ap_second.detectThreshold = 0.9;
	ap_second.cfgfile = (char*)cfgFile.c_str();
	ap_second.weightFile = (char*)weightFile2.c_str();

	if (initParames)
	{
		status = phone.InitAlgoriParam(
			IMAGE_WIDTH, IMAGE_HEIGHT, CHANNEL_NUMBER, *initParames) ? ERR_OK : ERR_BAD_OPERATE;
		if (ERR_OK == status)
		{
			int w = 0, h = 0;
			status = gpuDectect.InitSetup(IMAGE_WIDTH, IMAGE_HEIGHT, CHANNEL_NUMBER, ap_second, w, h) ? ERR_OK : ERR_BAD_ALLOC;
		}
	}

	return status;
}

void CVAlgoPhone::arithmeticWorkerProcess()
{
//	boost::winapi::ULONGLONG_ lastKnownTickTime{ 0 };

	while (1)
	{
		MediaImagePtr bgr24ImagePtr{ BGR24ImageQueue.remove() };

		if (bgr24ImagePtr)
		{
			std::vector<AlarmInfo> alarmInfos;
//			boost::winapi::ULONGLONG_ mainProcTime{ GetTickCount64() };
			phone.MainProcFunc((unsigned char*)bgr24ImagePtr->getImage(), feedback);
//			printf("=====  MainProcFunc run time = %lld.\r\n", GetTickCount64() - mainProcTime);

// 			typedef std::map<int, StruMemoryInfo>::iterator Iterator;
// 			for (Iterator it = feedback.mapMemory.begin(); it != feedback.mapMemory.end();)
// 			{
// // 				if (it->second.bDone)
// // 				{
// 					int nonePhoneNumber{ 0 }, havePhoneNumber{ 0 };
// 					float maxConfidence{ 0.0f };
// 					int nSaveId{ (int)(it->second.vecSaveMat.size() -1) };
// 
// 					for (int i = 0; i < it->second.vecSaveMat.size(); i++)
// 					{
// 						if (0 == it->second.vecSaveMat[i].nLabel)
// 						{
// 							havePhoneNumber++;
// 						}
// 						else
// 						{
// 							nonePhoneNumber++;
// 						}
// 
// // 						if (it->second.vecSaveMat[i].detectConfidence > maxConfidence)
// // 						{
// // 							maxConfidence = it->second.vecSaveMat[i].detectConfidence;
// // 							nSaveId = i;
// // 						}
// 					}
// 
// 					if (havePhoneNumber > nonePhoneNumber && -1 < nSaveId)
// 					{
// 						AlarmInfo alarmInfo;
// 						alarmInfo.type = AlarmType::ALARM_TYPE_PHONE;
// 						alarmInfo.x = it->second.vecSaveMat[nSaveId].rRect.x;
// 						alarmInfo.y = it->second.vecSaveMat[nSaveId].rRect.y;
// 						alarmInfo.w = it->second.vecSaveMat[nSaveId].rRect.width;
// 						alarmInfo.h = it->second.vecSaveMat[nSaveId].rRect.height;
// 						alarmInfo.status = it->second.vecSaveMat[nSaveId].nLabel;
// 						alarmInfos.push_back(alarmInfo);
// 
// 						bgr24ImagePtr->setOriginImage(
// 							(const unsigned char*)(it->second.vecSaveMat[nSaveId].pUcharImage), IMAGE_WIDTH * IMAGE_HEIGHT * 3);
// 						boost::checked_array_delete(it->second.vecSaveMat[nSaveId].pUcharImage);
// 						it = feedback.mapMemory.erase(it);
// 					}
// 					else
// 					{
// 						++it;
// 					}
// 
// 					if (0 < alarmInfos.size() && captureAlarmInfoHandler)
// 					{
// 						boost::winapi::ULONGLONG_ currentTickTime{ GetTickCount64() };
// 
// 						if (!lastKnownTickTime || 3000 < currentTickTime - lastKnownTickTime)
// 						{
// 							lastKnownTickTime = currentTickTime;
// 							captureAlarmInfoHandler(bgr24ImagePtr, alarmInfos);
// 						}
// 					}
// // 				}
// // 				else
// // 				{
// // 					++it;
// // 				}
// 			}

			for (std::map<int, StruMemoryInfo>::iterator iter = feedback.mapMemory.begin(); iter != feedback.mapMemory.end();)
			{
//				if (iter->second.bDone)
				{
					// 计算最大的detectConfidence 和 fTrackConfidence;
					int nPhoneNum = 0;
					int nZuojiNum = 0;
					float maxConfidence = 0.f;
					int nSaveId = 0;
					for (int i = 0; i < iter->second.vecSaveMat.size(); i++)
					{
						// 增加第二次识别过滤--对第一次检测的结果进行扩张后，进行第二次确认
						cv::Mat extractMat(IMAGE_HEIGHT, IMAGE_WIDTH, CV_8UC3, iter->second.vecSaveMat[i].pUcharImage);
						cv::Rect expandRect;
						expandRect.x = (cv::max)(0, int(iter->second.vecSaveMat[i].rRect.x - iter->second.vecSaveMat[i].rRect.width * 0.1));
						expandRect.y = (cv::max)(0, int(iter->second.vecSaveMat[i].rRect.y - iter->second.vecSaveMat[i].rRect.height * 0.1));
						expandRect.width = (cv::min)(extractMat.cols - expandRect.x, int(iter->second.vecSaveMat[i].rRect.width * 1.2));
						expandRect.height = (cv::min)(extractMat.rows - expandRect.y, int(iter->second.vecSaveMat[i].rRect.height * 1.2));

						if (0 > expandRect.x || 0 > expandRect.y || 0 > expandRect.width || 0 > expandRect.height)
						{
							continue;
						}

						cv::Mat roiMat = extractMat(expandRect).clone();
						std::vector<StruDetectResult> secondResult;
						gpuDectect.MultiObjectDetect(roiMat, secondResult);

						float widthScale2 = roiMat.cols * 1.0 / 416;
						float heightScale2 = roiMat.rows * 1.0 / 416;
						if (!secondResult.empty())
						{
							if (secondResult[0].nLabel == 0)
							{
								nPhoneNum++;
							}
							else
								nZuojiNum++;
						}
						else
							continue;

						if (iter->second.vecSaveMat[i].detectConfidence > maxConfidence&& secondResult[0].nLabel == 0)
						{
							maxConfidence = iter->second.vecSaveMat[i].detectConfidence;
							nSaveId = i;
						}
					}

					if (nPhoneNum > nZuojiNum) // 说明此组数据属于打电话，非座机，保存
					{
						AlarmInfo alarmInfo;
						alarmInfo.type = AlarmType::ALARM_TYPE_PHONE;
						alarmInfo.x = iter->second.vecSaveMat[nSaveId].rRect.x;
						alarmInfo.y = iter->second.vecSaveMat[nSaveId].rRect.y;
						alarmInfo.w = iter->second.vecSaveMat[nSaveId].rRect.width;
						alarmInfo.h = iter->second.vecSaveMat[nSaveId].rRect.height;
						alarmInfo.status = iter->second.vecSaveMat[nSaveId].nLabel;
						alarmInfos.push_back(alarmInfo);
						bgr24ImagePtr->setOriginImage(
							(const unsigned char*)(iter->second.vecSaveMat[nSaveId].pUcharImage), IMAGE_WIDTH * IMAGE_HEIGHT * 3);

						if (captureAlarmInfoHandler)
						{
							captureAlarmInfoHandler(bgr24ImagePtr, alarmInfos);
// 							boost::winapi::ULONGLONG_ currentTickTime{ GetTickCount64() };
// 
// 							if (!lastKnownTickTime || 3000 < currentTickTime - lastKnownTickTime)
// 							{
// 								lastKnownTickTime = currentTickTime;
// 								captureAlarmInfoHandler(bgr24ImagePtr, alarmInfos);
// 							}
 						}
					}

					for (int i = 0; i < iter->second.vecSaveMat.size(); i++)
					{
						if (nullptr != iter->second.vecSaveMat[i].pUcharImage)
							delete[] iter->second.vecSaveMat[i].pUcharImage;
					}
					iter = feedback.mapMemory.erase(iter);
				}
// 				else
// 					iter++;
			}
		}
		else
		{
			break;
		}
	}
}

NS_END