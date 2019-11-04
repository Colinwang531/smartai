#ifndef __FACEMODEL_H
#define __FACEMODEL_H

#include "../include/DefGlobalVar.h"
#include <vector>

using namespace std;

#ifndef DLL_EXPORT
#define DLL_EXPORT __declspec(dllexport)
#endif 

class DLL_EXPORT CFaceAlgorithmClass
{
public:
	CFaceAlgorithmClass();
	virtual ~CFaceAlgorithmClass();

	// 初始化算法，
	virtual bool InitModel(const int &width, const int &height, const int &channel, StruInitParams &param, CRITICAL_SECTION *csLock);

	// 传入当前图像路径和唯一id标签，返回特征值及人脸坐标、id信息
	virtual bool RegisterFace(char *szName, int id);

	// 算法分析：检测跟踪人脸
	virtual bool MainProcFunc(unsigned char *pClrImg, FeedBackFaceRecog &objFeed);

	// 算法后处理:识别人脸
	virtual bool PostProcessFunc(FeedBackFaceRecog &objFeed);

	// 实时检测人脸并提取特征值即人脸坐标信息 hitThreshold一般为3；hitThreshold=2时灵敏些，但可能有误捡
	virtual bool FaceDetectAndFeatureExtract(unsigned char *pClrImg, const int &width, const int &height, int hitThreshold, vector<StruFaceInfo> &faceInfo);

	// 人脸相似度比对
	virtual float FacePairMatch(unsigned char *pFeature1, unsigned char *pFeature2, const int &featureLength);

private:
	CFaceAlgorithmClass *m_pTreat;
};


#endif // #ifndef __DETECTOR_H
