#include "error.h"
#include "Arithmetic/CVAlgo.h"

NS_BEGIN(algo, 1)

boost::thread_group CVAlgo::threadGroup;

CVAlgo::CVAlgo() : stopped{ false }, totalFrameNumber{ 0 }
{}

CVAlgo::~CVAlgo()
{}

int CVAlgo::initialize()
{
	int status{ initializeArithmetic() };

	if (ERR_OK == status)
	{
		BGR24ImageQueue.setCapacity(12);
		//Each arithmetic run on different thread of CPU.
		boost::thread* threadHandle{ threadGroup.create_thread(boost::bind(&CVAlgo::arithmeticProcessThread, this)) };
 		if (threadHandle)
 		{
			threadHandle->detach();
 			SetThreadPriority(threadHandle->native_handle(), THREAD_PRIORITY_TIME_CRITICAL);
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

int CVAlgo::inputImageData(const unsigned char* imageData /* = NULL */, const int imageBytes /* = 0 */)
{
	int status{ imageData && 0 < imageBytes ? ERR_OK : ERR_INVALID_PARAM };

	if (ERR_OK == status && (++totalFrameNumber % 2 == 1))
	{
		MediaDataPtr mediaDataPtr{
			boost::make_shared<MediaData>(MediaDataMainID::MEDIA_DATA_MAIN_ID_IMAGE, MediaDataSubID::MEDIA_DATA_SUB_ID_BGR24) };

		if (mediaDataPtr && ERR_OK == mediaDataPtr->copyData(imageData, imageBytes))
		{
			BGR24ImageQueue.insert(mediaDataPtr);
		}
		else
		{
			status = ERR_BAD_ALLOC;
		}
	}

	return status;
}

void CVAlgo::getArithmeticInitParameter(StruInitParams& parameters, const AlarmType alarmType)
{
}

void CVAlgo::arithmeticProcessThread()
{
	while (!stopped)
	{
		arithmeticWorkerProcess();
		//Sleep(5);
	}

	while (1)
	{
		MediaDataPtr bgr24ImagePtr{ BGR24ImageQueue.remove() };
		if (!bgr24ImagePtr)
		{
			break;
		}
	}

	condition.notify_one();
}

NS_END
