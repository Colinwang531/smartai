//
//		Copyright :					@2018, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2019-07-19
//		Description :				CV打电话检测算法类
//
//		History:						Author										Date													Description
//											王科威										2019-08-26									创建
//

#ifndef CV_ALGO_PHONE_H
#define CV_ALGO_PHONE_H

#include "Arithmetic/CVAlgo.h"
#include "BasePhoneDll.h"

namespace framework
{
	namespace arithmetic
	{
		class CVAlgoPhone : public CVAlgo
		{
		public:
			CVAlgoPhone(void);
			~CVAlgoPhone(void);

		protected:
			int initializeArithmetic() override;
			int deinitializeArithmetic(void) override;
			void arithmeticWorkerProcess(void) override;

		private:
			CPhoneAlgorithmClass phone;
			FeedBackPhone feedback;
		};//class CVAlgoPhone
	}//namespace arithmetic
}//namespace framework

#endif//CV_ALGO_PHONE_H
