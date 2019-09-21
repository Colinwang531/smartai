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
{}

CVAlgoFace::~CVAlgoFace()
{}

bool CVAlgoFace::registerFace(StruFaceInfo& featureInfo, const char* fileName /* = NULL */)
{
	bool status{ false };

	if (fileName)
	{
		status = face.RegisterFace((char*)fileName, ++registerFaceID, &featureInfo);
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

	if(status)
	{
		status = face.InitModel();
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
				vector<StruFaceInfo> faceInfos;
				//			unsigned long long lastKnownTime = GetTickCount64();
				face.FaceDetectAndFeatureExtract((unsigned char*)frame->frameData, IMAGE_WIDTH, IMAGE_HEIGHT, CHANNEL_NUMBER, faceInfos);
// 				if (0 < faceInfos.size())
// 				{
// 					int x = 1;
// 				}
// 				unsigned long long currentTime = GetTickCount64();
// 				printf("FaceDetectAndFeatureExtract expire %I64u, person number %d.\r\n", currentTime - lastKnownTime, (int)faceInfos.size());
				for (int i = 0; i != faceInfos.size(); ++i)
				{
					DetectNotify detect;
					detect.type = ALGO_FACE;
					detect.status = 0;
					detect.x = faceInfos[i].faceRect.x;
					detect.y = faceInfos[i].faceRect.y;
					detect.w = faceInfos[i].faceRect.width;
					detect.h = faceInfos[i].faceRect.height;
					detect.face.similarity = 0;
					detect.face.imageBytes = 0;
					detect.face.imageData = NULL;
					detect.face.faceID = -1;

					float maxSimilarity = 0.0f;
					int maxFaceIndex = -1;
//					unsigned long long lastKnownTime = GetTickCount64();
					for (int j = 0; j != currentFaceFeatures.size(); ++j)
					{
						const float currentSimilarity{
							face.FacePairMatch(faceInfos[i].faceFeature, (unsigned char*)currentFaceFeatures[j].feature, FACE_FEATURE_LENGTH) };
						if (currentSimilarity >= 0.52f && currentSimilarity > maxSimilarity)
						{
							maxFaceIndex = j;
							maxSimilarity = currentSimilarity;
						}
					}
// 					unsigned long long currentTime = GetTickCount64();
// 					printf("FacePairMatch expire %I64u, similarity %f.\r\n", currentTime - lastKnownTime, maxSimilarity);

					if (-1 < maxFaceIndex)
					{
						try
						{
							detect.face.imageData = new char[1024 * 1024];
							detect.face.similarity = (int)(maxSimilarity * 100);
							detect.face.faceID = currentFaceFeatures[maxFaceIndex].id;
							FILE* jpegFile{ NULL };
							fopen_s(&jpegFile, (boost::format("%s\\Face\\%s_%d.jpg") % executePath % currentFaceFeatures[maxFaceIndex].name % currentFaceFeatures[maxFaceIndex].id).str().c_str(), "rb");
							if (jpegFile)
							{
								detect.face.imageBytes = _filelength(_fileno(jpegFile));
								fread(detect.face.imageData, 1024 * 1024, 1, jpegFile);
								fclose(jpegFile);
							}
							detectNotifies.push_back(detect);
						}
						catch (const std::exception&)
						{

						}
					}
				}

				if (captureAlarmNotifyHandler)
				{
					captureAlarmNotifyHandler(frame, detectNotifies);
				}

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