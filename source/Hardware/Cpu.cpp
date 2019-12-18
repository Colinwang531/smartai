#ifdef _WINDOWS
#include <windows.h>
#include "boost/winapi/system.hpp"
#else
#include <sys/sysinfo.h>
#include <sched.h>
#include <pthread.h>
#endif//_WINDOWS
#include "Hardware/Cpu.h"

namespace framework
{
	namespace hardware
	{
		Cpu::Cpu()
		{}

		Cpu::~Cpu()
		{}

		const unsigned char Cpu::getCoreNumber() const
		{
			unsigned char cores = 1;

#ifdef _WINDOWS
			boost::winapi::_SYSTEM_INFO info;
			boost::winapi::GetSystemInfo(&info);
			cores = static_cast<unsigned char>(info.dwNumberOfProcessors);
#else
			cores = get_nprocs();
#endif//_WINDOWS

			return cores;
		}

		void Cpu::setThreadAffinityMask(
#ifdef _WINDOWS
			void* tid /* = nullptr */,
#else
			unsigned long long tid /*= 0*/,
#endif//_WINDOWS
			const int mask /* = 0 */)
		{
#ifdef _WINDOWS
			const int tempThreadAffinityMask{ 0x01 << mask };
			SetThreadAffinityMask(tid, tempThreadAffinityMask);
#else
			cpu_set_t tempCpuSet;
			CPU_ZERO(&tempCpuSet);
			CPU_SET(mask, &tempCpuSet);
			status = pthread_setaffinity_np(tid, sizeof(cpu_set_t), &tempCpuSet);
#endif//_WINDOWS
		}
	}//namespace hardware
}//namespace framework
