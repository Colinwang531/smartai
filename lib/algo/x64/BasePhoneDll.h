#ifndef __PHONEMODEL_H
#define __PHONEMODEL_H

#include "DefGlobalVar.h"
#include <vector>

using namespace std;

#ifndef DLL_EXPORT
#define DLL_EXPORT __declspec(dllexport)
#endif 

class DLL_EXPORT CPhoneAlgorithmClass
{
public:
	CPhoneAlgorithmClass();
	virtual ~CPhoneAlgorithmClass();

	virtual bool clear_oldvec();
	// 初始化算法
	virtual bool InitAlgoriParam(const int &width, const int &height, const int &channel, StruInitParams &param);

	// 算法检测跟踪
	virtual bool MainProcFunc(unsigned char *pClrImg, FeedBackPhone &objFeed);

	// 算法后处理
	virtual bool PostProcessFunc(FeedBackPhone &objFeed);
	CPhoneAlgorithmClass *m_pTreat;
private:
	
};


#endif // #ifndef __DETECTOR_H
