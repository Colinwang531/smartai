#ifndef __HELMETMODEL_H
#define __HELMETMODEL_H

#include "../include/DefGlobalVar.h"
#include <vector>

using namespace std;

#ifndef DLL_EXPORT
#define DLL_EXPORT __declspec(dllexport)
#endif 

class DLL_EXPORT CHelmetAlgorithmClass
{
public:
	CHelmetAlgorithmClass();
	virtual ~CHelmetAlgorithmClass();

	// 初始化算法
	virtual bool InitAlgoriParam(const int &width, const int &height, const int &channel, StruInitParams &param);

	// 算法检测跟踪
	virtual bool MainProcFunc(unsigned char *pClrImg, FeedBackHelmet &objFeed);

	// 算法后处理
	virtual bool PostProcessFunc(FeedBackHelmet &objFeed);

private:
	CHelmetAlgorithmClass *m_pTreat;
};


#endif // #ifndef __DETECTOR_H
