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

	//�������
	virtual bool clear_oldvec();
	// ��ʼ���㷨
	virtual bool InitAlgoriParam(const int &width, const int &height, const int &channel, StruInitParams &param);

	// �㷨������
	virtual bool MainProcFunc(unsigned char *pClrImg, FeedBackFight &objFeed);

	// �㷨����
	virtual bool PostProcessFunc(FeedBackFight &objFeed);
	CFightAlgorithmClass *m_pTreat;
private:
	
};


#endif // #ifndef __DETECTOR_H
