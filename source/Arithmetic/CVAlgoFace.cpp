#include <windows.h>
#include <io.h>
#include "boost/filesystem.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/checked_delete.hpp"
#include "boost/filesystem.hpp"
#include "boost/format.hpp"
#include "error.h"
#include "Arithmetic/CVAlgoFace.h"

namespace framework
{
	namespace arithmetic
	{
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
				status = face.RegisterFace((char*)filePath, faceID) ? ERR_OK : ERR_BAD_OPERATE;

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
			static boost::winapi::ULONGLONG_ lastKnownTickTime_test{ GetTickCount64() };

			while (1)
			{
				FeedBackFaceRecog feedback;
				MediaDataPtr bgr24ImagePtr{ BGR24ImageQueue.remove() };

				if (bgr24ImagePtr)
				{
					std::vector</*FaceInfo*/AlarmInfo> faceInfos;
					boost::winapi::ULONGLONG_ nowProcTime{ GetTickCount64() };
					face.MainProcFunc((unsigned char*)bgr24ImagePtr->getData(), feedback);
					printf("=====  MainProcFunc run time = %lld.\r\n", nowProcTime - lastKnownTickTime_test);
					lastKnownTickTime_test = nowProcTime;
					printf("=====  face.mapMeory.size() = %d.\r\n", (int)feedback.mapMemory.size());

					if (face.PostProcessFunc(feedback))
					{
						//				printf("=====  face.vecFaceResult.size() = %d.\r\n", (int)feedback.vecFaceResult.size());
						for (int j = 0; j != feedback.vecFaceResult.size(); ++j)
						{
							/*FaceInfo*/AlarmInfo faceInfo;
							faceInfo.type = AlarmType::ALARM_TYPE_FACE;
							faceInfo.similarity = feedback.vecFaceResult[j].similarValue;
							faceInfo.faceID = feedback.vecFaceResult[j].matchId;

							// 					mtx.lock();
							// 					boost::unordered_map<int, const std::string>::iterator it{
							// 						registerFaceImageGroup.find(feedback.vecFaceResult[j].matchId) };
							// 					if (registerFaceImageGroup.end() != it)
							// 					{
							// 						FILE* fd{ NULL };
							// 						fopen_s(&fd, it->second.c_str(), "rb+");
							// 						if (fd)
							// 						{
							// 							faceInfo.imageBytes = _filelength(_fileno(fd));
							// 							faceInfo.imageData = new(std::nothrow) char[faceInfo.imageBytes];
							// 							if (faceInfo.imageData)
							// 							{
							// 								fread(faceInfo.imageData, faceInfo.imageBytes, 1, fd);
							// 							}
							// 							fclose(fd);
							// 							std::vector<std::string> faceImageFileNameSegment;
							// 							boost::split(faceImageFileNameSegment, it->second, boost::is_any_of("_"));
							// 							faceInfo.faceID = atoll(faceImageFileNameSegment[1].c_str());
							// 						}
							// 					}
							// 					mtx.unlock();

							faceInfos.push_back(faceInfo);
							boost::checked_array_delete(feedback.vecFaceResult[j].pUcharImage);
						}

						feedback.vecFaceResult.clear();
					}

					if (0 < faceInfos.size() && postDetectAlarmInfoCallback)
					{
						//				capturefaceInfoHandler(bgr24ImagePtr, faceInfos);
										//boost::winapi::ULONGLONG_ currentTickTime{ GetTickCount64() };

										//if (!lastKnownTickTime || 3000 < currentTickTime - lastKnownTickTime)
										//{
										//	lastKnownTickTime = currentTickTime;
										//	capturefaceInfoHandler(bgr24ImagePtr, faceInfos);
										//}
						postDetectAlarmInfoCallback(faceInfos[0], NULL, 0);
					}

					feedback.vecShowInfo.clear();
				}
				else
				{
					break;
				}
			}
		}
	}//namespace arithmetic
}//namespace framework
