#include "zmq.h"
#include "error.h"
#include "MessageQueue/MQThread.h"

NS_BEGIN(mq, 1)

MQThread::MQThread() : threadID{ NULL }, stopped{ true }
{}

MQThread::~MQThread()
{}

int MQThread::start()
{
	int status{ ERR_EXISTED };

	if (!threadID)
	{
		threadID = zmq_threadstart(&MQThread::threadFunc, this);
		stopped = false;
		status = threadID ? ERR_OK : ERR_BAD_OPERATE;
	}
	
	return status;
}

void MQThread::stop()
{
	stopped = true;
	boost::unique_lock<boost::mutex> lock{ mtx };
	condition.wait(lock);

	if (threadID)
	{
		zmq_threadclose(threadID);
	}
}

void MQThread::threadFunc(void* ctx /* = NULL */)
{
	MQThread* thread{ reinterpret_cast<MQThread*>(ctx) };

	if (thread)
	{
		thread->process();
		thread->condition.notify_one();
	}
}

NS_END
