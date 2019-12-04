//
//		Copyright :					@2018, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2019-07-19
//		Description :				CV˯������㷨��
//
//		History:						Author										Date													Description
//											������										2019-08-26									����
//

#ifndef CV_ALGO_SLEEP_H
#define CV_ALGO_SLEEP_H

#include "Arithmetic/CVAlgo.h"
#include "BaseSleepDll.h"

namespace framework
{
	namespace arithmetic
	{
		class CVAlgoSleep : public CVAlgo
		{
		public:
			CVAlgoSleep(void);
			~CVAlgoSleep(void);

		protected:
			int initializeArithmetic() override;
			int deinitializeArithmetic(void) override;
			void arithmeticWorkerProcess(void) override;

		private:
			CSleepAlgorithmClass sleep;
			FeedBackSleep feedback;
		};//class CVAlgoSleep
	}//namespace arithmetic
}//namespace framework

#endif//CV_ALGO_SLEEP_H
