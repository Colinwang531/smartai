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

	// ��ʼ���㷨
	virtual bool InitAlgoriParam(const int &width, const int &height, const int &channel, StruInitParams &param_first, StruInitParams &param_second);

	// �㷨������
	virtual bool MainProcFunc(unsigned char *pClrImg, FeedBackPhone &objFeed);

	// �㷨����
	virtual bool PostProcessFunc(FeedBackPhone &objFeed);

private:
	CPhoneAlgorithmClass *m_pTreat;
};


#endif // #ifndef __DETECTOR_H
