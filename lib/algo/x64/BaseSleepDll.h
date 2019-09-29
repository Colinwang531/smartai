#ifndef __SLEEPMODEL_H
#define __SLEEPMODEL_H

#include "DefGlobalVar.h"
#include <vector>

using namespace std;

#ifndef DLL_EXPORT
#define DLL_EXPORT __declspec(dllexport)
#endif 

class DLL_EXPORT CSleepAlgorithmClass
{
public:
	CSleepAlgorithmClass();
	virtual ~CSleepAlgorithmClass();

	virtual bool clear_oldvec();
	// ��ʼ���㷨
	virtual bool InitAlgoriParam(const int &width, const int &height, const int &channel, StruInitParams &param);

	// �㷨������
	virtual bool MainProcFunc(unsigned char *pClrImg, FeedBackSleep &objFeed);

	// �㷨����
	virtual bool PostProcessFunc(FeedBackSleep &objFeed);
	CSleepAlgorithmClass *m_pTreat;
private:
	//CSleepAlgorithmClass *m_pTreat;
};


#endif // #ifndef __DETECTOR_H
