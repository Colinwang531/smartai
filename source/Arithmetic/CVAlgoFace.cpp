#include <windows.h>
#include <io.h>
#include "boost/algorithm/string.hpp"
#include "boost/checked_delete.hpp"
#include "boost/filesystem.hpp"
#include "boost/format.hpp"
#include "error.h"
#include "Arithmetic/CVAlgoFace.h"

NS_BEGIN(algo, 1)

CVAlgoFace::CVAlgoFace(CaptureAlarmNotifyHandler handler /* = NULL */)
	: CVAlgo(handler), largestRegisterFaceID{ 0 }
{
	InitializeCriticalSection(&criticalSection);
}

CVAlgoFace::~CVAlgoFace()
{
	DeleteCriticalSection(&criticalSection);
}

int CVAlgoFace::addFace(
	const char* jpegData /* = NULL */, const int jpegBytes /* = 0 */, const long long uuid /* = -1 */, const char* name /* = NULL */)
{
	int status{ ERR_INVALID_PARAM };

	if (jpegData && 0 < jpegBytes && 0 < uuid && name)
	{
		const std::string executePath{
				boost::filesystem::initial_path<boost::filesystem::path>().string() };
		const std::string jpegFileName{
			(boost::format("%s\\Face\\%lld_%lld_%s.jpg") % executePath % ++largestRegisterFaceID % uuid % name).str() };

		FILE* jpegFile{ NULL };
		fopen_s(&jpegFile, jpegFileName.c_str(), "wb+");
		if (jpegFile)
		{
			fwrite(jpegData, jpegBytes, 1, jpegFile);
			fclose(jpegFile);
			status = face.RegisterFace(const_cast<char*>(jpegFileName.c_str()), largestRegisterFaceID) ? ERR_OK : ERR_BAD_OPERATE;

			if (ERR_OK == status)
			{
				mtx.lock();
				registerFaceImageGroup.insert(std::make_pair(largestRegisterFaceID, jpegFileName));
				mtx.unlock();
			}
		}
	}

	return status;
}

int CVAlgoFace::removeFace(const long long uuid /* = -1 */)
{
	int status{ ERR_NOT_FOUND };

	mtx.lock();
	for(boost::unordered_map<int, const std::string>::iterator it = registerFaceImageGroup.begin(); it != registerFaceImageGroup.end(); ++it)
	{
		std::string uuidStr{ (boost::format("%lld") % uuid).str() };
		const std::string faceImagePath{ it->second };
		const char* isSubstr{ std::strstr(faceImagePath.c_str(), uuidStr.c_str()) };

		if (isSubstr)
		{
			registerFaceImageGroup.erase(it);
			std::remove(faceImagePath.c_str());
			break;
		}
	}
	mtx.unlock();

	return status;
}

int CVAlgoFace::queryFace(const long long uuid, char*& jpegData, int& jpegBytes)
{
	mtx.lock();
	for (boost::unordered_map<int, const std::string>::iterator it = registerFaceImageGroup.begin(); it != registerFaceImageGroup.end(); ++it)
	{
		std::string uuidStr{ (boost::format("%lld") % uuid).str() };
		const char* isSubstr{ std::strstr(it->second.c_str(), uuidStr.c_str()) };

		if (isSubstr)
		{
			FILE* jpegFile{ NULL };
			fopen_s(&jpegFile, it->second.c_str(), "rb+");
			
			if (jpegFile)
			{
				const long jpegDataBytes{ _filelength(_fileno(jpegFile)) };
				if (jpegData)
				{
					fread(jpegData, jpegDataBytes, 1, jpegFile);
					jpegBytes = jpegDataBytes;
				}

				fclose(jpegFile);
			}

			break;
		}
	}
	mtx.unlock();

	return ERR_OK;
}

bool CVAlgoFace::initializeWithParameter(const char* configFilePath /* = NULL */, void* parameter /* = NULL */)
{
	const std::string cfgFile{ (boost::format("%s\\model\\face.cfg") % configFilePath).str() };
	const std::string weightFile{ (boost::format("%s\\model\\face.weights") % configFilePath).str() };
	StruInitParams* initParames{ reinterpret_cast<StruInitParams*>(parameter) };
	initParames->cfgfile = (char*)cfgFile.c_str();
	initParames->weightFile = (char*)weightFile.c_str();
	initParames->matchThreshold = 0.52f;
	bool status{ face.InitModel(IMAGE_WIDTH, IMAGE_HEIGHT, CHANNEL_NUMBER, *initParames, &criticalSection) };

	if (status)
	{
		loadAndRegisterFacePicture(configFilePath);
	}

	return status;
}

void CVAlgoFace::algorithmWorkerProcess()
{
	while (1)
	{
		livestreamMtx.lock();
		LivestreamGroup livestreams{ livestreamGroup };
		livestreamMtx.unlock();

		for (boost::unordered_map<const std::string, LivestreamPtr>::iterator it = livestreams.begin(); it != livestreams.end(); it++)
		{
			FeedBackFaceRecog faceDetect, feedback;
			std::vector<void*> bgr24FrameQueue;
			it->second->queue(ALGO_FACE, bgr24FrameQueue);

			for (std::vector<void*>::iterator it = bgr24FrameQueue.begin(); it != bgr24FrameQueue.end();)
			{
				BGR24Frame* frame{ reinterpret_cast<BGR24Frame*>(*it) };
				std::vector<DetectNotify> detectNotifiers;
				DWORD mainProcStart = GetTickCount64();
				bool result{ face.MainProcFunc((unsigned char*)frame->frameData, feedback) };
//				printf("MainProcFunc cost %d.\r\n", (int)(GetTickCount64() - mainProcStart));

				for (int i = 0; i != feedback.vecShowInfo.size(); ++i)
				{
					DWORD postProcStart = GetTickCount64();
					if (face.PostProcessFunc(feedback))
					{
//						printf("PostProcFunc cost %d.\r\n", (int)(GetTickCount64() - postProcStart));
						faceDetect = feedback;
						for (int j = 0; j != faceDetect.vecFaceResult.size(); ++j)
						{
							DetectNotify detect;
							detect.type = ALGO_FACE;
							detect.status = 0;
							detect.x = faceDetect.vecShowInfo[i].rRect.x;
							detect.y = faceDetect.vecShowInfo[i].rRect.y;
							detect.w = faceDetect.vecShowInfo[i].rRect.width;
							detect.h = faceDetect.vecShowInfo[i].rRect.height;
							detect.face.similarity = faceDetect.vecFaceResult[j].similarValue;

							mtx.lock();
							boost::unordered_map<int, const std::string>::iterator it = registerFaceImageGroup.find(faceDetect.vecFaceResult[j].matchId/*detect.face.faceID*/);
							if (registerFaceImageGroup.end() != it)
							{
								FILE* jpegFile{ NULL };
								fopen_s(&jpegFile, it->second.c_str(), "rb");
								if (jpegFile)
								{
									detect.face.imageBytes = _filelength(_fileno(jpegFile));
									detect.face.imageData = new(std::nothrow) char[detect.face.imageBytes];
									if (detect.face.imageData)
									{
										fread(detect.face.imageData, detect.face.imageBytes, 1, jpegFile);
									}
									fclose(jpegFile);

									std::vector<std::string> faceImageFileNameSegment;
									boost::split(faceImageFileNameSegment, it->second, boost::is_any_of("_"));
									detect.face.faceID = atoll(faceImageFileNameSegment[1].c_str());
								}
							}
							mtx.unlock();

							detectNotifiers.push_back(detect);
//							boost::checked_array_delete(faceDetect.vecFaceResult[j].pUcharImage);
							faceDetect.vecShowInfo.clear();
							faceDetect.mapMemory.clear();
						}
					}
				}

				if (captureAlarmNotifyHandler && 0 < detectNotifiers.size())
				{
					captureAlarmNotifyHandler(frame, detectNotifiers);
				}

				boost::checked_array_delete(frame->frameData);
				boost::checked_array_delete(frame->jpegData);
				boost::checked_array_delete(frame->NVRIp);
				boost::checked_delete(frame);
				for (int i = 0; i != detectNotifiers.size(); ++i)
				{
					boost::checked_array_delete(detectNotifiers[i].face.imageData);
				}
				it = bgr24FrameQueue.erase(it);
			}
		}
	}
}

int CVAlgoFace::loadAndRegisterFacePicture(const char* directoryFilePath /* = NULL */)
{
	boost::filesystem::path recursiveDirPath((boost::format("%s\\Face") % directoryFilePath).str());
	boost::filesystem::recursive_directory_iterator endIter;

	for (boost::filesystem::recursive_directory_iterator it(recursiveDirPath); it != endIter; ++it)
	{
		if (!boost::filesystem::is_directory(*it))
		{
			const std::string faceImageFileName{ it->path().filename().string() };

			if (!faceImageFileName.empty())
			{
				std::vector<std::string> faceImageFileNameSegment;
				boost::split(faceImageFileNameSegment, faceImageFileName, boost::is_any_of("_"));
				const int currentRegisterFaceID{ atoi(faceImageFileNameSegment[0].c_str()) };
				registerFaceImageGroup.insert(std::make_pair(currentRegisterFaceID, it->path().string()));

				//Keep largestRegisterFaceID value is the newest to use for next time.
				if (currentRegisterFaceID > largestRegisterFaceID)
				{
					largestRegisterFaceID = currentRegisterFaceID;
				}

				const std::string jpegFileFullPath{ (boost::format("%s\\face\\%s") % directoryFilePath % faceImageFileName).str() };
				if (face.RegisterFace(const_cast<char*>(jpegFileFullPath.c_str()), currentRegisterFaceID))
				{
					registerFaceImageGroup.insert(std::make_pair(currentRegisterFaceID, jpegFileFullPath.c_str()));
				}
			}
		}
	}

	return ERR_OK;
}

NS_END