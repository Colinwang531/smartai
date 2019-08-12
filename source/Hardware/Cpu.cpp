#ifdef _WINDOWS
#include <windows.h>
#include "boost/winapi/system.hpp"
#else
#include <sys/sysinfo.h>
#include <sched.h>
#include <pthread.h>
#endif//_WINDOWS
#include "error.h"
#include "Hardware/Cpu.h"

NS_BEGIN(hardware, 1)

Cpu::Cpu()
{}

Cpu::~Cpu()
{}

const int Cpu::getCPUCoreNumber() const
{
	int tempNumberOfCores = 1;

#ifdef _WINDOWS
	boost::winapi::_SYSTEM_INFO info;
	boost::winapi::GetSystemInfo(&info);
	tempNumberOfCores = info.dwNumberOfProcessors;
#else
	tempNumberOfCores = get_nprocs();
#endif//_WINDOWS

	return tempNumberOfCores;
}

int Cpu::setThreadAffinityMask(
#ifdef _WINDOWS
	void* tid /* = nullptr */, 
#else
	unsigned long long tid /*= 0*/,
#endif//_WINDOWS
	const int mask /* = 0 */)
{
	int status{ ERR_OK };

#ifdef _WINDOWS
	const int tempThreadAffinityMask{ 0x01 << mask };
	status = static_cast<int>(SetThreadAffinityMask(tid, tempThreadAffinityMask));
#else
	cpu_set_t tempCpuSet;
	CPU_ZERO(&tempCpuSet);
	CPU_SET(mask, &tempCpuSet);
	status = pthread_setaffinity_np(tid, sizeof(cpu_set_t), &tempCpuSet);
#endif//_WINDOWS

	return status;
}

NS_END
