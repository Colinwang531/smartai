#include "zmq.h"
#include "error.h"
#include "MessageQueue/MQThread.h"

NS_BEGIN(mq, 1)

MQThread::MQThread() : threadID{ NULL }
{}

MQThread::~MQThread()
{}

int MQThread::start()
{
	int status{ ERR_EXISTED };

	if (!threadID)
	{
		threadID = zmq_threadstart(&MQThread::threadFunc, this);
		status = threadID ? ERR_OK : ERR_BAD_OPERATE;
	}
	
	return status;
}

void MQThread::stop()
{
	//必须判断thread句柄是否有效,ZMQ内部没有检查就直接类型转换并调用了stop方法
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
	}
}

NS_END
