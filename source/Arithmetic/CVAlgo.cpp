#include <windows.h>
#include "boost/format.hpp"
#include "DefGlobalVar.h"
#include "Arithmetic/CVAlgo.h"

NS_BEGIN(algo, 1)

int CVAlgo::algoNumber = 0;

CVAlgo::CVAlgo(FIFOList* fqueue /* = NULL */, CVAlgoDetectNotifyHandler handler /* = NULL */)
	: frameQueue{ fqueue }, cvAlgoDetectNotifyHandler{ handler },
	imageWidth{ 0 }, imageHeight{ 0 }, channelNumber{ 0 }
{}

CVAlgo::~CVAlgo()
{}

bool CVAlgo::initialize(
	const char* path /* = NULL */, const float detectThreshold /* = 0.0f */, const float trackThreshold /* = 0.0f */,
	const int width /* = 1920 */, const int height /* = 1080 */, const int channel /* = 3 */)
{
	imageWidth = width;
	imageHeight = height;
	channelNumber = channel;
	const std::string cfgFile{ (boost::format("%s\\model\\fight.cfg") % path).str() };
	const std::string weightFile{ (boost::format("%s\\model\\fight.weights") % path).str() };
	StruInitParams parameters{};
	parameters.detectThreshold = detectThreshold;
	parameters.trackThreshold = trackThreshold;
	parameters.cfgfile = (char*)cfgFile.c_str();
	parameters.weightFile = (char*)weightFile.c_str();

	bool status{ initializeWithParameter(&parameters) };

	if (status)
	{
		HANDLE handle = (HANDLE)_beginthreadex(NULL, 0, &mainWorkerThread, this, 0, NULL);
		SetThreadPriority(handle, THREAD_PRIORITY_TIME_CRITICAL);
		SetThreadAffinityMask(handle, (DWORD)(1 << algoNumber));
		algoNumber++;

		handle = (HANDLE)_beginthreadex(NULL, 0, &subWorkerThread, this, 0, NULL);
		SetThreadPriority(handle, THREAD_PRIORITY_NORMAL);
		SetThreadAffinityMask(handle, (DWORD)(1 << algoNumber));
		algoNumber++;
	}

	return status;
}

void CVAlgo::subWorkerProcess()
{}

unsigned int CVAlgo::mainWorkerThread(void* ctx /* = NULL */)
{
	CVAlgo* cvalgo{ reinterpret_cast<CVAlgo*>(ctx) };

	if (cvalgo)
	{
		cvalgo->mainWorkerProcess();
	}

	return 0;
}

unsigned int CVAlgo::subWorkerThread(void* ctx /* = NULL */)
{
	CVAlgo* cvalgo{ reinterpret_cast<CVAlgo*>(ctx) };

	if (cvalgo)
	{
		cvalgo->subWorkerProcess();
	}

	return 0;
}


NS_END