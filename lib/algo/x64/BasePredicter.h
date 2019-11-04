#ifndef __KLAMANPREDICT_H
#define __KLAMANPREDICT_H

#ifndef DLL_EXPORT
#define DLL_EXPORT __declspec(dllexport)
#endif 

#include <list>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

typedef struct _StateSpaceModel
{
	bool bHasDone;
	int nIteration;
	Mat m_matA, m_matC, m_matQ, m_matP, m_matHat;
	list<float> dxHist, dyHist;

	_StateSpaceModel()
	{
		bHasDone = false;
		nIteration = 1;

		float AData[] = { 1, 0, 1, 0,
			0, 1, 0, 1,
			0, 0, 1, 0,
			0, 0, 0, 1 };

		m_matA = Mat(4, 4, CV_32FC1, AData).clone();

		float CData[] = { 1, 0, 0, 0, 0, 1, 0, 0 };
		m_matC = Mat(2, 4, CV_32FC1, CData).clone();

		float posCov = 10.0f;
		float velCov = 5.0f;
		float PData[] = { posCov, 0, 0, 0,
			0, posCov, 0, 0,
			0, 0, velCov, 0,
			0, 0, 0, velCov };
		m_matP = Mat(4, 4, CV_32FC1, PData).clone();

		float posVar = 200.0f;
		float velVar = 10.01f;
		float QData[] = { posVar, 0, 0, 0,
			0, posVar, 0, 0,
			0, 0, velVar, 0,
			0, 0, 0, velVar };
		m_matQ = Mat(4, 4, CV_32FC1, QData).clone();

		float x0Data[] = { 0, 0, 0, 0 };
		m_matHat = Mat(4, 1, CV_32FC1, x0Data).clone();
	}
}StateSpaceModel;

typedef struct _PredictParam
{
	float offsetX;   // 偏移量
	float offsetY;
	float predictCenterX;//预测目标中心
	float predictCenterY;
	StateSpaceModel stateModel;
	_PredictParam()
	{
		offsetX = offsetY = predictCenterX = predictCenterY = 0.f;
	}
}PredictParam;

class DLL_EXPORT CKalmanPredict
{
public:
	CKalmanPredict();
	virtual ~CKalmanPredict();

	virtual bool InitKalman(); 
	virtual bool PredictObjectsPosByKalman(PredictParam &objPredict);

private:
	CKalmanPredict *m_pTreat;
};


#endif