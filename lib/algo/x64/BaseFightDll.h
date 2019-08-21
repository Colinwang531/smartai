#ifndef __FIGHTMODEL_H
#define __FIGHTMODEL_H

#include "DefGlobalVar.h"
#include <vector>

using namespace std;

#ifndef DLL_EXPORT
#define DLL_EXPORT __declspec(dllexport)
#endif 

class DLL_EXPORT CFightAlgorithmClass
{
public:
	CFightAlgorithmClass();
	virtual ~CFightAlgorithmClass();

	// 初始化算法
	virtual bool InitAlgoriParam(const int &width, const int &height, const int &channel, StruInitParams &param);

	// 算法检测跟踪
	virtual bool MainProcFunc(unsigned char *pClrImg, FeedBackFight &objFeed);

	// 算法后处理
	virtual bool PostProcessFunc(FeedBackFight &objFeed);

private:
	CFightAlgorithmClass *m_pTreat;
};


#endif // #ifndef __DETECTOR_H
