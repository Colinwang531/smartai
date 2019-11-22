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

#include "boost/thread/mutex.hpp"
#include "boost/unordered_map.hpp"
#include "Arithmetic/CVAlgo.h"
#include "BaseFaceDll.h"

NS_BEGIN(algo, 1)

class CVAlgoFace : public CVAlgo
{
public:
	CVAlgoFace(void);
	~CVAlgoFace(void);

public:
	int addFacePicture(const char* filePath = NULL, const int faceID = 0);
	int removeFacePicture(const long long uuid = -1);

protected:
	int initializeArithmetic() override;
	int deinitializeArithmetic(void) override;
	void arithmeticWorkerProcess(void) override;

private:
//	int loadAndRegisterFacePicture(const char* directoryFilePath = NULL);

private:
	CFaceAlgorithmClass face;
	CRITICAL_SECTION criticalSection;
//	int largestRegisterFaceID;
	boost::unordered_map<int, const std::string> registerFaceImageGroup;
};//class CVAlgoFace

NS_END

#endif//CV_ALGO_FACE_H
