#ifndef __HELMETMODEL_H
#define __HELMETMODEL_H

#include "DefGlobalVar.h"
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

	// ��ʼ���㷨
	virtual bool clear_oldvec();
	virtual bool InitAlgoriParam(const int &width, const int &height, const int &channel, StruInitParams &param);

	// �㷨������
	virtual bool MainProcFunc(unsigned char *pClrImg, FeedBackHelmet &objFeed);

	// �㷨����
	virtual bool PostProcessFunc(FeedBackHelmet &objFeed);
	CHelmetAlgorithmClass *m_pTreat;

private:
	//CHelmetAlgorithmClass *m_pTreat;
};


#endif // #ifndef __DETECTOR_H
