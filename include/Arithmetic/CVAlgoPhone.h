//
//		Copyright :					@2018, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2019-07-19
//		Description :				CV��绰����㷨��
//
//		History:						Author										Date													Description
//											������										2019-08-26									����
//

#ifndef CV_ALGO_PHONE_H
#define CV_ALGO_PHONE_H

#include "Arithmetic/CVAlgo.h"
#include "BasePhoneDll.h"

NS_BEGIN(algo, 1)

class CVAlgoPhone : public CVAlgo
{
public:
	CVAlgoPhone(CaptureAlarmNotifyHandler handler = NULL);
	~CVAlgoPhone(void);

protected:
	bool initializeWithParameter(void* parameter = NULL) override;
	void algorithmWorkerProcess(void) override;

private:
	CPhoneAlgorithmClass phone;
};//class CVAlgoPhone

NS_END

#endif//CV_ALGO_PHONE_H
