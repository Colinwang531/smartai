//
//		Copyright :					@2018, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2019-07-19
//		Description :				CV��ܼ���㷨��
//
//		History:						Author										Date													Description
//											������										2019-08-26									����
//

#ifndef CV_ALGO_FIGHT_H
#define CV_ALGO_FIGHT_H

#include "Arithmetic/CVAlgo.h"
#include "BaseFightDll.h"

NS_BEGIN(algo, 1)

class CVAlgoFight : public CVAlgo
{
public:
	CVAlgoFight(void);
	~CVAlgoFight(void);

protected:
	int initializeArithmetic() override;
	int deinitializeArithmetic(void) override;
	void arithmeticWorkerProcess(void) override;

private:
	CFightAlgorithmClass fight;
	FeedBackFight feedback;
};//class CVAlgoFight

NS_END

#endif//CV_ALGO_FIGHT_H
