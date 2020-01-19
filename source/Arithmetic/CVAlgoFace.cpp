#include <windows.h>
#include <io.h>
#include "boost/filesystem.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/checked_delete.hpp"
#include "boost/filesystem.hpp"
#include "boost/format.hpp"
#include "opencv2/opencv.hpp"
#include "error.h"
#include "Arithmetic/CVAlgoFace.h"

NS_BEGIN(algo, 1)

CVAlgoFace::CVAlgoFace() : CVAlgo()
{
	InitializeCriticalSection(&criticalSection);
}

CVAlgoFace::~CVAlgoFace()
{
	DeleteCriticalSection(&criticalSection);
}

int CVAlgoFace::addFacePicture(const char* filePath /* = NULL */, const int faceID /* = 0 */)
{
	int status{ ERR_INVALID_PARAM };

	if (filePath && 0 < faceID)
	{
		mtx.lock();
		status = face.RegisterFace((char*)filePath, faceID) ? ERR_OK : ERR_BAD_OPERATE;
		mtx.unlock();

		if (ERR_OK == status)
		{
			registerFaceImageGroup.insert(std::make_pair(faceID, filePath));
		}
	}

	return status;
}

int CVAlgoFace::removeFacePicture(const long long uuid /* = -1 */)
{
	int status{ ERR_NOT_FOUND };

// 	mtx.lock();
// 	for(boost::unordered_map<int, const std::string>::iterator it = registerFaceImageGroup.begin(); it != registerFaceImageGroup.end(); ++it)
// 	{
// 		std::string uuidStr{ (boost::format("%lld") % uuid).str() };
// 		const std::string faceImagePath{ it->second };
// 		const char* isSubstr{ std::strstr(faceImagePath.c_str(), uuidStr.c_str()) };
// 
// 		if (isSubstr)
// 		{
// 			registerFaceImageGroup.erase(it);
// 			std::remove(faceImagePath.c_str());
// 			break;
// 		}
// 	mtx.unlock();

	return status;
}

int CVAlgoFace::initializeArithmetic()
{
	const std::string executePath{
			boost::filesystem::initial_path<boost::filesystem::path>().string() };
	const std::string cfgFile{ (boost::format("%s\\model\\face.cfg") % executePath).str() };
	const std::string weightFile{ (boost::format("%s\\model\\face.weights") % executePath).str() };
	StruInitParams parameters;
	parameters.gpu_id = 0;
	parameters.detectThreshold = 0.20f;
	parameters.trackThreshold = 0.15f;
	parameters.matchThreshold = 0.52f;
	parameters.cfgfile = (char*)cfgFile.c_str();
	parameters.weightFile = (char*)weightFile.c_str();

	return face.InitModel(
		IMAGE_WIDTH, IMAGE_HEIGHT, CHANNEL_NUMBER, parameters, &criticalSection) ? ERR_OK : ERR_BAD_OPERATE;
}

int CVAlgoFace::deinitializeArithmetic()
{
	return ERR_OK;
}

void CVAlgoFace::arithmeticWorkerProcess()
{
//	boost::winapi::ULONGLONG_ lastKnownTickTime{ 0 };
//	static boost::winapi::ULONGLONG_ lastKnownTickTime_test{ GetTickCount64() };

	while (1)
	{
		FeedBackFaceRecog feedback;
		MediaDataPtr bgr24ImagePtr{ BGR24ImageQueue.remove() };

		if (bgr24ImagePtr)
		{
			mtx.lock();

			std::vector</*FaceInfo*/AlarmInfo> faceInfos;
//			boost::winapi::ULONGLONG_ nowProcTime{ GetTickCount64() };
			face.MainProcFunc((unsigned char*)bgr24ImagePtr->getData(), feedback);
//			printf("=====  MainProcFunc run time = %lld.\r\n", nowProcTime - lastKnownTickTime_test);
//			lastKnownTickTime_test = nowProcTime;
//			printf("=====  face.mapMeory.size() = %d.\r\n", (int)feedback.mapMemory.size());

			if (face.PostProcessFunc(feedback))
			{
//				printf("=====  face.vecFaceResult.size() = %d.\r\n", (int)feedback.vecFaceResult.size());

// 				if (0 < feedback.vecFaceResult.size())
// 				{
// 					char text[2048]{ 0 };
// 					sprintf_s(text, 2048, "=====  face.vecFaceResult.size() = %d.\r\n", (int)feedback.vecFaceResult.size());
// 					OutputDebugStringA(text);
// 				}
				
				for (int j = 0; j != feedback.vecFaceResult.size(); ++j)
				{
					/*FaceInfo*/AlarmInfo faceInfo;
					faceInfo.type = AlarmType::ALARM_TYPE_FACE;
					faceInfo.similarity = feedback.vecFaceResult[j].similarValue;
					faceInfo.faceID = feedback.vecFaceResult[j].matchId;
					faceInfo.trackID = feedback.vecFaceResult[j].nSerioNo;
					faceInfo.x = feedback.vecShowInfo[j].rRect.x;
					faceInfo.y = feedback.vecShowInfo[j].rRect.y;
					faceInfo.w = feedback.vecShowInfo[j].rRect.width;
					faceInfo.h = feedback.vecShowInfo[j].rRect.height;

					cv::Mat faceProcessed;
					if (feedback.vecFaceResult[j].ingChannel == 3)
						faceProcessed = cv::Mat(feedback.vecFaceResult[j].imgHeight, feedback.vecFaceResult[j].imgWidth, CV_8UC3, feedback.vecFaceResult[j].pUcharImage);
					else if (feedback.vecFaceResult[j].ingChannel == 1)
						faceProcessed = cv::Mat(feedback.vecFaceResult[j].imgHeight, feedback.vecFaceResult[j].imgWidth, CV_8UC1, feedback.vecFaceResult[j].pUcharImage);
					vector<uchar> buff;
					cv::imencode(".jpg", faceProcessed, buff);
					std::string strJPGData{ buff.begin(), buff.end() };
					faceInfo.imageBytes = strJPGData.length();
					faceInfo.faceImage = new(std::nothrow) unsigned char[faceInfo.imageBytes];
					if (faceInfo.faceImage)
					{
						memcpy_s(faceInfo.faceImage, faceInfo.imageBytes, strJPGData.c_str(), faceInfo.imageBytes);
					}

					faceInfos.push_back(faceInfo);
					boost::checked_array_delete(feedback.vecFaceResult[j].pUcharImage);
				}

				feedback.vecFaceResult.clear();
			}

			if (0 < faceInfos.size() && postDetectAlarmInfoCallback)
			{
				postDetectAlarmInfoCallback(faceInfos, bgr24ImagePtr->getData(), bgr24ImagePtr->getDataBytes());

				for (int i = 0; i != faceInfos.size(); ++i)
				{
					boost::checked_array_delete(faceInfos[i].faceImage);
				}
			}

			 feedback.vecShowInfo.clear();

			 mtx.unlock();
		}
		else
		{
			break;
		}
	}
}

//int CVAlgoFace::loadAndRegisterFacePicture(const char* directoryFilePath /* = NULL */)
//{
//	boost::filesystem::path recursiveDirPath((boost::format("%s\\Face") % directoryFilePath).str());
//	boost::filesystem::recursive_directory_iterator endIter;
//
//	for (boost::filesystem::recursive_directory_iterator it(recursiveDirPath); it != endIter; ++it)
//	{
//		if (!boost::filesystem::is_directory(*it))
//		{
//			const std::string faceImageFileName{ it->path().filename().string() };
//
//			if (!faceImageFileName.empty())
//			{
//				std::vector<std::string> faceImageFileNameSegment;
//				boost::split(faceImageFileNameSegment, faceImageFileName, boost::is_any_of("_"));
//				const int currentRegisterFaceID{ atoi(faceImageFileNameSegment[0].c_str()) };
//				registerFaceImageGroup.insert(std::make_pair(currentRegisterFaceID, it->path().string()));
//
//				//Keep largestRegisterFaceID value is the newest to use for next time.
//				if (currentRegisterFaceID > largestRegisterFaceID)
//				{
//					largestRegisterFaceID = currentRegisterFaceID;
//				}
//
//				const std::string jpegFileFullPath{ (boost::format("%s\\face\\%s") % directoryFilePath % faceImageFileName).str() };
//				if (face.RegisterFace(const_cast<char*>(jpegFileFullPath.c_str()), currentRegisterFaceID))
//				{
//					mtx.lock();
//					registerFaceImageGroup.insert(std::make_pair(currentRegisterFaceID, jpegFileFullPath.c_str()));
//					mtx.unlock();
//				}
//			}
//		}
//	}
//
//	return ERR_OK;
//}

NS_END