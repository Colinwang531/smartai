#ifndef __YOLODETECTION_H
#define __YOLODETECTION_H

#include "../include/DefGlobalVar.h"
#include "opencv2/opencv.hpp"

#ifndef GPUDLL_EXPORT
#define GPUDLL_EXPORT __declspec(dllexport)
#endif 


struct StruDetectResult
{
	int nLabel;
	float fConfidence;
	cv::Rect rRect;
};

class GPUDLL_EXPORT CGpuDetect
{
public:
	CGpuDetect();
	virtual ~CGpuDetect();

	virtual bool InitSetup(const int &width, const int &height, const int &channel, StruInitParams &param, int &processWidth, int &processHeight);
	virtual bool MultiObjectDetect(cv::Mat srcMat, std::vector<StruDetectResult> &detectResult);
	virtual void ReleaseMem();

private:
	CGpuDetect *m_pTreat;
};


#endif
