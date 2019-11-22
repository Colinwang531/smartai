// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Base class of arithmetic for helmet detecting.
//

#ifndef CV_ALGO_HELMET_H
#define CV_ALGO_HELMET_H

#include "Arithmetic/CVAlgo.h"
#include "BaseHelmetDll.h"

NS_BEGIN(algo, 1)

class CVAlgoHelmet : public CVAlgo
{
public:
	CVAlgoHelmet(void);
	~CVAlgoHelmet(void);

protected:
	int initializeArithmetic() override;
	int deinitializeArithmetic(void) override;
	void arithmeticWorkerProcess(void) override;

private:
	CHelmetAlgorithmClass helmet;
	FeedBackHelmet feedback;
};//class CVAlgoHelmet

NS_END

#endif//CV_ALGO_HELMET_H
