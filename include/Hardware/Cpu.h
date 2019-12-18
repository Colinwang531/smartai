// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Base class of CPU information.
//

#ifndef CPU_H
#define CPU_H

namespace framework
{
	namespace hardware
	{
		class Cpu
		{
		public:
			Cpu(void);
			~Cpu(void);

			const unsigned char getCoreNumber(void) const;
			void setThreadAffinityMask(
#ifdef _WINDOWS
				void* tid = nullptr,
#else
				unsigned long long tid = 0,
#endif//_WINDOWS
				const int mask = 0);
		};//class Cpu
	}//namespace hardware
}//namespace framework

#endif//CPU_H
