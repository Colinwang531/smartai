#ifndef __MULTITRACKERMODEL_H
#define __MULTITRACKERMODEL_H

#include <vector>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

#ifndef DLL_EXPORT
#define DLL_EXPORT __declspec(dllexport)
#endif 

typedef struct _TrackerParam
{
	Mat _alphaf;
	Mat _prob;
	Mat _tmpl;
	Size _tmpl_sz;
	float m_scale;
	int size_patch[3];
	Mat hann;
	Rect rRect;
	Rect_<float> m_roi;
	_TrackerParam()
	{
		m_scale = 0.f;
		size_patch[0] = size_patch[1] = size_patch[2] = 0;
		rRect = Rect(0, 0, 0, 0);
		m_roi = Rect(0.f, 0.f, 0.f, 0.f);
	}
}TrackerParam;

class DLL_EXPORT CMultiTracker
{
public:
	CMultiTracker();
	virtual ~CMultiTracker();

	virtual bool InitParam();

	// 初始化算法
	virtual bool InitTracker(Mat matClrImg, TrackerParam &objParam);

	// 算法跟踪
	virtual float UpdateTracker(Mat matClrImg, TrackerParam &objParam);

private:
	CMultiTracker *m_pTreat;
};


#endif // #ifndef __DETECTOR_H
