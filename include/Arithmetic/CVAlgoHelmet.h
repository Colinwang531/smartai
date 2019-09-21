//
//		Copyright :					@2018, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2019-07-19
//		Description :				CV��ȫñ����㷨��
//
//		History:						Author										Date													Description
//											������										2019-08-26									����
//

#ifndef CV_ALGO_HELMET_H
#define CV_ALGO_HELMET_H

#include "Arithmetic/CVAlgo.h"
#include "BaseHelmetDll.h"

NS_BEGIN(algo, 1)

class CVAlgoHelmet : public CVAlgo
{
public:
	CVAlgoHelmet(CaptureAlarmNotifyHandler handler = NULL);
	~CVAlgoHelmet(void);

protected:
	bool initializeWithParameter(
		const char* configFilePath = NULL, void* parameter = NULL) override;
	void algorithmWorkerProcess(void) override;

private:
	CHelmetAlgorithmClass helmet;
};//class CVAlgoHelmet

NS_END

#endif//CV_ALGO_HELMET_H
