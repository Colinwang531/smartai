#include <windows.h>
#include <io.h>
#include "boost/algorithm/string.hpp"
#include "boost/checked_delete.hpp"
#include "boost/filesystem/path.hpp"
#include "boost/filesystem/operations.hpp"
#include "boost/format.hpp"
#include "tinyxml2.h"
using namespace tinyxml2;
#include "error.h"
#include "Arithmetic/CVAlgoFace.h"

NS_BEGIN(algo, 1)

CVAlgoFace::CVAlgoFace(CaptureAlarmNotifyHandler handler /* = NULL */)
	: CVAlgo(handler), registerFaceID{ 0 }
{
	InitializeCriticalSection(&criticalSection);
}

CVAlgoFace::~CVAlgoFace()
{
	DeleteCriticalSection(&criticalSection);
}

bool CVAlgoFace::registerFace(StruFaceInfo& featureInfo, const char* fileName /* = NULL */)
{
	bool status{ false };

	if (fileName)
	{
		status = face.RegisterFace((char*)fileName, ++registerFaceID);
	}

	return status;
}

bool CVAlgoFace::addFaceFeature(
	const char* name /* = NULL */, const long long id /* = -1 */, const unsigned char* feature /* = NULL */)
{
	bool status{ false };

	if (name && feature && -1 < id)
	{
		FaceFeature faceFeature{ 0 };
		faceFeature.id = id;
		int namelen{ (int)strlen(name) };
		memcpy_s(faceFeature.name, namelen, name, namelen);
		memcpy_s(faceFeature.feature, FACE_FEATURE_LENGTH, feature, FACE_FEATURE_LENGTH);

		mtx.lock();
		faceFeatures.push_back(faceFeature);
		mtx.unlock();

		const std::string executePath{
			boost::filesystem::initial_path<boost::filesystem::path>().string() };
		const std::string featureFileName{
			(boost::format("%s\\Face\\Feature.xml") % executePath).str() };
		std::string faceFeatureStr;

		for (int i = 0; i != FACE_FEATURE_LENGTH; ++i)
		{
			char item[4]{ 0 };
			_itoa_s(feature[i], item, 4, 16);
			faceFeatureStr.append(item);
			faceFeatureStr.append(",");
		}
	
		XMLDocument doc;
		doc.LoadFile(featureFileName.c_str());
		XMLElement* root = doc.RootElement();
		XMLElement* e = doc.NewElement("person");
		e->SetAttribute("id", id);
		e->SetAttribute("feature", faceFeatureStr.c_str());
		e->SetText(faceFeature.name);
		root->InsertEndChild(e);
		doc.SaveFile(featureFileName.c_str());

 		status = true;
	}

	return status;
}

bool CVAlgoFace::removeFaceFeature(const char* name /* = NULL */, const long long id /* = -1 */)
{
	bool status{false};
	const std::string executePath{
			boost::filesystem::initial_path<boost::filesystem::path>().string() };
	const std::string featureFileName{
		(boost::format("%s\\Face\\Feature.xml") % executePath).str() };

	XMLDocument doc;
	doc.LoadFile(featureFileName.c_str());
	XMLElement* root = doc.RootElement();
	XMLElement* e = root->FirstChildElement();

	while(e)
	{
		long long elementID{-1};
		const std::string elementName{e->GetText()};
		const XMLAttribute* attr = e->FirstAttribute();

		while(attr)
		{
			const std::string attrName{attr->Name()};
			if(!attrName.compare("id"))
			{
				elementID = atoi(attr->Value());
				break;
			}

			attr = attr->Next();
		}

		if(!elementName.compare(name) && elementID == id)
		{
			root->DeleteChild(e);
			status = true;
			break;
		}
		else
		{
			e = e->NextSiblingElement();
		}
	}

	doc.SaveFile(featureFileName.c_str());

	mtx.lock();
	for(std::vector<FaceFeature>::iterator it = faceFeatures.begin(); it != faceFeatures.end(); ++it)
	{
		if(!strcmp(it->name, name) && it->id == id)
		{
			faceFeatures.erase(it);
			break;
		}
	}
	mtx.unlock();

	return status;
}

bool CVAlgoFace::initializeWithParameter(const char* configFilePath /* = NULL */, void* parameter /* = NULL */)
{
	bool status{ loadFaceFeature() };
	const std::string cfgFile{ (boost::format("%s\\model\\face.cfg") % configFilePath).str() };
	const std::string weightFile{ (boost::format("%s\\model\\face.weights") % configFilePath).str() };
	StruInitParams* initParames{ reinterpret_cast<StruInitParams*>(parameter) };
	initParames->cfgfile = (char*)cfgFile.c_str();
	initParames->weightFile = (char*)weightFile.c_str();

	if(status)
	{
		initParames->matchThreshold = 0.6f;
		status = face.InitModel(IMAGE_WIDTH, IMAGE_HEIGHT, CHANNEL_NUMBER, *initParames, &criticalSection);

		if (status)
		{
			WIN32_FIND_DATAA ffd;
// 			LARGE_INTEGER filesize;
// 			TCHAR szDir[MAX_PATH];
// 			size_t length_of_arg;
//			HANDLE hFind = INVALID_HANDLE_VALUE;
//			DWORD dwError = 0;

			const std::string jpegFilePath{ (boost::format("%s\\face\\*") % configFilePath).str() };
			HANDLE hFind{ FindFirstFileA(jpegFilePath.c_str(), &ffd) };
			BOOL findResult{ TRUE };

			while (INVALID_HANDLE_VALUE != hFind && findResult)
			{
				if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
				}
				else
				{
					const std::string faceFileName{ ffd.cFileName };
					const std::size_t dotCharPos{ faceFileName.rfind('.') };
					const std::string faceFileAppendix{ faceFileName.substr(dotCharPos + 1, 3) };

					if (!faceFileAppendix.compare("jpg"))
					{
						const std::string faceFilePath{ (boost::format("%s\\face\\%s") % configFilePath % ffd.cFileName).str() };
						face.RegisterFace((char*)(faceFilePath.c_str()), ++registerFaceID);
					}
				}

				findResult = FindNextFileA(hFind, &ffd);
			}

			FindClose(hFind);
		}
	}

	return status;
}

void CVAlgoFace::algorithmWorkerProcess()
{
	const std::string executePath{
		boost::filesystem::initial_path<boost::filesystem::path>().string() };

	while (1)
	{
		for (boost::unordered_map<const std::string, LivestreamPtr>::iterator it = livestreamGroup.begin(); it != livestreamGroup.end(); it++)
		{
			std::vector<void*> bgr24FrameQueue;
			it->second->queue(ALGO_HELMET, bgr24FrameQueue);

			for (std::vector<void*>::iterator it = bgr24FrameQueue.begin(); it != bgr24FrameQueue.end();)
			{
				BGR24Frame* frame{ reinterpret_cast<BGR24Frame*>(*it) };
				if (!frame)
				{
					break;
				}
				
				mtx.lock();
				const std::vector<FaceFeature> currentFaceFeatures{ faceFeatures };
				mtx.unlock();

				vector<DetectNotify> detectNotifies;
//				vector<StruFaceInfo> faceInfos;
				//			unsigned long long lastKnownTime = GetTickCount64();
				//face.FaceDetectAndFeatureExtract((unsigned char*)frame->frameData, IMAGE_WIDTH, IMAGE_HEIGHT, CHANNEL_NUMBER, faceInfos);

				FeedBackFaceRecog faceDetect;
				bool result{ face.MainProcFunc((unsigned char*)frame->frameData, faceDetect) };
// 				unsigned long long currentTime = GetTickCount64();
// 				printf("FaceDetectAndFeatureExtract expire %I64u, person number %d.\r\n", currentTime - lastKnownTime, (int)faceInfos.size());

				for (int i = 0; i != faceDetect.vecShowInfo.size(); ++i)
				{
// 					DetectNotify detect;
// 					detect.type = ALGO_FACE;
// 					detect.status = 0;
// 					detect.x = faceDetect.vecShowInfo[i].rRect.x;
// 					detect.y = faceDetect.vecShowInfo[i].rRect.y;
// 					detect.w = faceDetect.vecShowInfo[i].rRect.width;
// 					detect.h = faceDetect.vecShowInfo[i].rRect.height;
// 					detect.face.similarity = 0;
// 					detect.face.imageBytes = 0;
// 					detect.face.imageData = NULL;
// //					detect.face.faceID = faceDetect.vecShowInfo[i].nSerioNo;
// 
// 					detect.face.imageData = new char[1024 * 1024];
// 					detect.face.similarity = 0;// (int)(maxSimilarity * 100);
// 					detect.face.faceID = currentFaceFeatures[faceDetect.vecShowInfo[i].nSerioNo].id;
// 					FILE* jpegFile{ NULL };
// 					fopen_s(&jpegFile, (boost::format("%s\\Face\\%s_%d.jpg") % executePath % currentFaceFeatures[faceDetect.vecShowInfo[i].nSerioNo].name % currentFaceFeatures[faceDetect.vecShowInfo[i].nSerioNo].id).str().c_str(), "rb");
// 					if (jpegFile)
// 					{
// 						detect.face.imageBytes = _filelength(_fileno(jpegFile));
// 						fread(detect.face.imageData, 1024 * 1024, 1, jpegFile);
// 						fclose(jpegFile);
// 					}
// 
// 					detectNotifies.push_back(detect);
				}

// 				if (captureAlarmNotifyHandler && 0 < detectNotifies.size())
// 				{
// 					captureAlarmNotifyHandler(frame, detectNotifies);
// 				}

				boost::checked_array_delete(frame->frameData);
				boost::checked_array_delete(frame->jpegData);
				boost::checked_array_delete(frame->NVRIp);
				boost::checked_delete(frame);
				for (int i = 0; i != detectNotifies.size(); ++i)
				{
					boost::checked_array_delete(detectNotifies[i].face.imageData);
				}
				it = bgr24FrameQueue.erase(it);
			}
		}
	}
}

bool CVAlgoFace::loadFaceFeature()
{
	FaceFeature ff;
	const std::string executePath{
		boost::filesystem::initial_path<boost::filesystem::path>().string() };
	const std::string featureFileName{
		(boost::format("%s\\Face\\Feature.xml") % executePath).str() };
	
	XMLDocument doc;
	doc.LoadFile(featureFileName.c_str());
	XMLElement* root = doc.RootElement();
	XMLElement* e = root->FirstChildElement();

	while(e)
	{
		strcpy_s(ff.name, e->GetText());
		const XMLAttribute* attr = e->FirstAttribute();

		while(attr)
		{
			const std::string attrName{attr->Name()};
			if(!attrName.compare("id"))
			{
				ff.id = atoll(attr->Value());
			}
			else if(!attrName.compare("feature"))
			{
				const std::string faceFeatureStr{ attr->Value() };
				std::vector<std::string> faceFeatureDatas;
				boost::split(faceFeatureDatas, faceFeatureStr, boost::is_any_of(","));

				for (int i = 0; i != FACE_FEATURE_LENGTH; ++i)
				{
					ff.feature[i] = (unsigned char)strtoul(faceFeatureDatas[i].c_str(), NULL, 16);
				}
			}

			attr = attr->Next();
		}

		e = e->NextSiblingElement();
		faceFeatures.push_back(ff);
	}

	doc.SaveFile(featureFileName.c_str());
	return true;
}

NS_END