//
//		Copyright :					@2018, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2019-07-19
//		Description :				CV打架检测算法类
//
//		History:						Author										Date													Description
//											王科威										2019-08-26									创建
//

#ifndef CV_ALGO_FIGHT_H
#define CV_ALGO_FIGHT_H

#include "Arithmetic/CVAlgo.h"
#include "BaseFightDll.h"

NS_BEGIN(algo, 1)

class CVAlgoFight : public CVAlgo
{
public:
	CVAlgoFight(CaptureAlarmNotifyHandler handler = NULL);
	~CVAlgoFight(void);

protected:
	bool initializeWithParameter(void* parameter = NULL) override;
	void algorithmWorkerProcess(void) override;

private:
	CFightAlgorithmClass fight;
};//class CVAlgoFight

NS_END

#endif//CV_ALGO_FIGHT_H
