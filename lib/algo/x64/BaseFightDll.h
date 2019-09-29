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

	//清空特征
	virtual bool clear_oldvec();
	// 初始化算法
	virtual bool InitAlgoriParam(const int &width, const int &height, const int &channel, StruInitParams &param);

	// 算法检测跟踪
	virtual bool MainProcFunc(unsigned char *pClrImg, FeedBackFight &objFeed);

	// 算法后处理
	virtual bool PostProcessFunc(FeedBackFight &objFeed);
	CFightAlgorithmClass *m_pTreat;
private:
	
};


#endif // #ifndef __DETECTOR_H
