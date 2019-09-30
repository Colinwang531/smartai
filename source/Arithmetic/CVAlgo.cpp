#include "DefGlobalVar.h"
#include "error.h"
#include "Arithmetic/CVAlgo.h"

NS_BEGIN(algo, 1)

DWORD CVAlgo::enableAlgorithmCount = 0;

CVAlgo::CVAlgo(CaptureAlarmNotifyHandler handler /* = NULL */) : captureAlarmNotifyHandler{ handler }
{}

CVAlgo::~CVAlgo()
{}

bool CVAlgo::initialize(
	const char* configFilePath /* = NULL */, const float detectThreshold /* = 0.0f */, const float trackThreshold /* = 0.0f */)
{
	StruInitParams parameters{};
	parameters.detectThreshold = detectThreshold;
	parameters.trackThreshold = trackThreshold;
	parameters.savePath = (char*)configFilePath;

	bool status{ initializeWithParameter(configFilePath, &parameters) };

	if (status)
	{
		//Each algorithm run on different thread of CPU.
		HANDLE threadHandle = (HANDLE)CreateThread(NULL, 0, &algorithmProcessThread, this, 0, NULL);
 		if (threadHandle)
 		{
 			SetThreadPriority(threadHandle, THREAD_PRIORITY_TIME_CRITICAL);
 			SetThreadAffinityMask(threadHandle, (DWORD)(1 << enableAlgorithmCount));
 			enableAlgorithmCount++;
 		}
	}

	return status;
}

DWORD CVAlgo::algorithmProcessThread(void* ctx /* = NULL */)
{
	CVAlgo* cvalgo{ reinterpret_cast<CVAlgo*>(ctx) };

	if (cvalgo)
	{
		cvalgo->algorithmWorkerProcess();
	}

	return 0;
}

int CVAlgo::addLivestream(const std::string streamID, LivestreamPtr livestreamPtr)
{
	int status{ ERR_EXISTED };

	livestreamMtx.lock();
	LivestreamGroup::const_iterator it = livestreamGroup.find(streamID);
	if (livestreamGroup.end() == it)
	{
		livestreamGroup.insert(std::make_pair(streamID, livestreamPtr));
		status = ERR_OK;
	}
	livestreamMtx.unlock();

	return status;
}

int CVAlgo::removeLivestream(const std::string streamID)
{
	int status{ ERR_NOT_FOUND };

	livestreamMtx.lock();
	LivestreamGroup::iterator it = livestreamGroup.find(streamID);
	if (livestreamGroup.end() != it)
	{
		livestreamGroup.erase(it);
		status = ERR_OK;
	}
	livestreamMtx.unlock();

	return status;
}

NS_END
