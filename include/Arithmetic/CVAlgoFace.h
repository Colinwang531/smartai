//
//		Copyright :					@2018, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2019-07-19
//		Description :				CV人脸检测算法类
//
//		History:						Author										Date													Description
//											王科威										2019-08-26									创建
//

#ifndef CV_ALGO_FACE_H
#define CV_ALGO_FACE_H

#include <vector>
#include "boost/thread/mutex.hpp"
#include "Arithmetic/CVAlgo.h"
#include "BaseFaceDll.h"

NS_BEGIN(algo, 1)

enum
{
	FACE_NAME_LENGTH = 260
};

typedef struct tagFaceFeature_t
{
	long long id;
	char name[FACE_NAME_LENGTH];
	unsigned char feature[FACE_FEATURE_LENGTH];
}FaceFeature;

class CVAlgoFace : public CVAlgo
{
public:
	CVAlgoFace(CaptureAlarmNotifyHandler handler = NULL);
	~CVAlgoFace(void);

public:
	bool registerFace(StruFaceInfo& featureInfo, const char* fileName = NULL);
	bool addFaceFeature(
		const char* name = NULL, const long long id = -1, const unsigned char* feature = NULL);
	bool removeFaceFeature(const char* name = NULL, const long long id = -1);

protected:
	bool initializeWithParameter(
		const char* configFilePath = NULL, void* parameter = NULL) override;
	void algorithmWorkerProcess(void) override;

private:
	bool loadFaceFeature(void);

private:
	CFaceAlgorithmClass face;
	int registerFaceID;
	std::vector<FaceFeature> faceFeatures;
	boost::mutex mtx;
};//class CVAlgoHelmet

NS_END

#endif//CV_ALGO_HELMET_H
