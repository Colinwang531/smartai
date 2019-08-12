//
//		Copyright :					@2017, HHJT, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2017-09-15
//		Description:					CPU基类
//
//		History:						Author										Date													Description
//											王科威										2017-09-15									创建
//

#ifndef CPU_H
#define CPU_H

#include "predef.h"

NS_BEGIN(hardware, 1)

class Cpu
{
public:
	Cpu(void);
	~Cpu(void);

	const int getCPUCoreNumber (void) const;
	int setThreadAffinityMask(
#ifdef _WINDOWS
		void* tid = nullptr, 
#else
		unsigned long long tid = 0,
#endif//_WINDOWS
		const int mask = 0);
};//class Cpu

NS_END

#endif//CPU_H
