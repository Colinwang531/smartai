#include "zmq.h"
#include "error.h"
#include "MessageQueue/MQThread.h"

NS_BEGIN(mq, 1)

MQThread::MQThread() : threadID{ NULL }
{}

MQThread::~MQThread()
{}

int MQThread::startThread(void* handler /* = NULL */, void* ctx /* = NULL */)
{
	int status{ ERR_INVALID_PARAM };

	if (handler)
	{
		threadID = zmq_threadstart(reinterpret_cast<zmq_thread_fn*>(handler), ctx);
		status = threadID ? ERR_OK : ERR_BAD_OPERATE;
	}
	
	return status;
}

void MQThread::stopThread()
{
	//�����ж�thread����Ƿ���Ч,ZMQ�ڲ�û�м���ֱ������ת����������stop����
	if (threadID)
	{
		zmq_threadclose(threadID);
	}
}

NS_END
