#include "DefGlobalVar.h"
#include "error.h"
#include "Arithmetic/CVAlgo.h"

NS_BEGIN(algo, 1)

//DWORD CVAlgo::enableAlgorithmCount = 0;
//extern DWORD threadAffinityMask;

CVAlgo::CVAlgo(
	CaptureAlarmInfoHandler alarmHandler /* = NULL */, CaptureFaceInfoHandler faceHandler /* = NULL */)
	: captureAlarmInfoHandler{ alarmHandler }, capturefaceInfoHandler{ faceHandler }, arithmeticProcessing{ false }, stopped{ false },
	lastKnownTickTime{ 0 }
{}

CVAlgo::~CVAlgo()
{}

int CVAlgo::initialize(
	const char* configFilePath /* = NULL */, const int affinityMask /* = 1 */, 
	const float detectThreshold /* = 0.0f */, const float trackThreshold /* = 0.0f */, 
	const int gpu /* = 0 */)
{
	int status{ ERR_BAD_OPERATE };
	StruInitParams parameters{};
	parameters.gpu_id = gpu;
	parameters.detectThreshold = detectThreshold;
	parameters.trackThreshold = trackThreshold;
	parameters.savePath = (char*)configFilePath;
	BGR24ImageQueue.setCapacity(12);

	if (initializeWithParameter(configFilePath, &parameters))
	{
		//Each arithmetic run on different thread of CPU.
		HANDLE handle = (HANDLE)CreateThread(NULL, 0, &arithmeticProcessThread, this, 0, NULL);
 		if (handle)
 		{
 			SetThreadPriority(handle, THREAD_PRIORITY_TIME_CRITICAL);
//			SetThreadAffinityMask(handle, affinityMask);
 		}

		status = ERR_OK;
	}

	return status;
}

void CVAlgo::deinitialize(void)
{
	if (!stopped)
	{
		stopped = true;
		boost::unique_lock<boost::mutex> lock{ mtx };
		condition.wait_for(lock, boost::chrono::seconds(3));
	}
}

int CVAlgo::tryInputMediaImage(MediaImagePtr image)
{
	int status{ ERR_INVALID_PARAM };

	if (image)
	{
		if (!arithmeticProcessing)
		{
			BGR24ImageQueue.insert(image);
			status = ERR_OK;

			if (BGR24ImageQueue.total() == BGR24ImageQueue.size())
			{
				arithmeticProcessing = true;
			}
		}
		else
		{
			status = ERR_BAD_OPERATE;
		}
	}

	return status;
}

DWORD CVAlgo::arithmeticProcessThread(void* ctx /* = NULL */)
{
	CVAlgo* cvalgo{ reinterpret_cast<CVAlgo*>(ctx) };

	while (cvalgo && !cvalgo->stopped)
	{
		if (cvalgo->arithmeticProcessing)
		{
			cvalgo->arithmeticWorkerProcess();
			cvalgo->arithmeticProcessing = false;
		}
		else
		{
			Sleep(1);
		}

// 		cvalgo->arithmeticWorkerProcess();
// 		Sleep(1);
	}

	while (cvalgo)
	{
		MediaImagePtr bgr24ImagePtr{ cvalgo->BGR24ImageQueue.remove() };
		if (!bgr24ImagePtr)
		{
			break;
		}
	}

	if (cvalgo)
	{
		cvalgo->condition.notify_one();
	}

	return 0;
}

NS_END
