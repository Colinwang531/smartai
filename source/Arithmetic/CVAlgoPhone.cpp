#include "boost/format.hpp"
#include "boost/winapi/time.hpp"
#include "boost/checked_delete.hpp"
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
			IMAGE_WIDTH, IMAGE_HEIGHT, CHANNEL_NUMBER, *initParames, ap_second) ? ERR_OK : ERR_BAD_OPERATE;
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

			map<int, StruMemoryInfo>::iterator iter = objFeed.mapMemory.begin();
			for (; iter != objFeed.mapMemory.end(); )
			{
				if (iter->second.bDone)
				{
					// ��������detectConfidence �� fTrackConfidence;
					int nPhoneNum = 0;
					int nZuojiNum = 0;
					float maxConfidence = 0.f;
					int nSaveId = 0;
					for (int i = 0; i < iter->second.vecSaveMat.size(); i++)
					{
						// ���ӵڶ���ʶ�����--�Ե�һ�μ��Ľ���������ź󣬽��еڶ���ȷ��
						Mat extractMat(m_nHeight, m_nWidth, CV_8UC3, iter->second.vecSaveMat[i].pUcharImage);
						Rect expandRect;
						expandRect.x = cv::max(0, int(iter->second.vecSaveMat[i].rRect.x - iter->second.vecSaveMat[i].rRect.width * 0.1));
						expandRect.y = cv::max(0, int(iter->second.vecSaveMat[i].rRect.y - iter->second.vecSaveMat[i].rRect.height * 0.1));
						expandRect.width = cv::min(extractMat.cols - expandRect.x, int(iter->second.vecSaveMat[i].rRect.width * 1.2));
						expandRect.height = cv::min(extractMat.rows - expandRect.y, int(iter->second.vecSaveMat[i].rRect.height * 1.2));

						Mat roiMat = extractMat(expandRect).clone();
						std::vector<StruDetectResult> secondResult;
						m_secondDetect->MultiObjectDetect(roiMat, secondResult);

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

					if (nPhoneNum > nZuojiNum) // ˵�������������ڴ�绰��������������
					{
						Mat extractMat(m_nHeight, m_nWidth, CV_8UC3, iter->second.vecSaveMat[nSaveId].pUcharImage);
						rectangle(extractMat, Rect(iter->second.vecSaveMat[nSaveId].rRect.x, iter->second.vecSaveMat[nSaveId].rRect.y,
							iter->second.vecSaveMat[nSaveId].rRect.width, iter->second.vecSaveMat[nSaveId].rRect.height), CV_RGB(0, 255, 0), 2, 8, 0);
						putText(extractMat, "phone", Point(iter->second.vecSaveMat[nSaveId].rRect.x, iter->second.vecSaveMat[nSaveId].rRect.y),
							FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(0, 255, 0), 1, CV_AA);

						char szName[128] = { 0 };
						time_t t;
						struct tm* lt;
						time(&t);//��ȡUnixʱ�����
						lt = localtime(&t);//תΪʱ��ṹ��
						sprintf_s(szName, "%s/%s_%03d_%4d_%02d_%02d_%02d_%02d_%02d.jpg", m_pSavePath, "phone", iter->second.vecSaveMat[nSaveId].nSerioNo,
							lt->tm_year + 1900, lt->tm_mon + 1, lt->tm_mday, lt->tm_hour, lt->tm_min, lt->tm_sec);
						imwrite(szName, extractMat);

						for (int i = 0; i < iter->second.vecSaveMat.size(); i++)
						{
							if (nullptr != iter->second.vecSaveMat[i].pUcharImage)
								delete[] iter->second.vecSaveMat[i].pUcharImage;
						}
					}
					else
					{
						Mat extractMat(m_nHeight, m_nWidth, CV_8UC3, iter->second.vecSaveMat[nSaveId].pUcharImage);
						rectangle(extractMat, Rect(iter->second.vecSaveMat[nSaveId].rRect.x, iter->second.vecSaveMat[nSaveId].rRect.y,
							iter->second.vecSaveMat[nSaveId].rRect.width, iter->second.vecSaveMat[nSaveId].rRect.height), CV_RGB(0, 255, 0), 2, 8, 0);
						putText(extractMat, "tele", Point(iter->second.vecSaveMat[nSaveId].rRect.x, iter->second.vecSaveMat[nSaveId].rRect.y),
							FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(0, 255, 0), 1, CV_AA);

						char szName[128] = { 0 };
						time_t t;
						struct tm* lt;
						time(&t);//��ȡUnixʱ�����
						lt = localtime(&t);//תΪʱ��ṹ��
						sprintf_s(szName, "%s/%s_%03d_%4d_%02d_%02d_%02d_%02d_%02d.jpg", m_pSavePath, "tele", iter->second.vecSaveMat[nSaveId].nSerioNo,
							lt->tm_year + 1900, lt->tm_mon + 1, lt->tm_mday, lt->tm_hour, lt->tm_min, lt->tm_sec);
						imwrite(szName, extractMat);

						for (int i = 0; i < iter->second.vecSaveMat.size(); i++)
						{
							if (nullptr != iter->second.vecSaveMat[i].pUcharImage)
								delete[] iter->second.vecSaveMat[i].pUcharImage;
						}
					}

					iter = objFeed.mapMemory.erase(iter);
				}
				else
					iter++;
			}
		}
		else
		{
			break;
		}
	}
}

NS_END