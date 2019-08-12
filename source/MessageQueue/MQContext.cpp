#include "zmq.h"
#include "error.h"
#include "MessageQueue/MQContext.h"

NS_BEGIN(mq, 1)

MQContext::MQContext() : zmqContext{ NULL }
{}

MQContext::~MQContext()
{}

int MQContext::initialize(const unsigned char threadNumber /*= 1*/)
{
	int status{ ERR_BAD_OPERATE };

	if (!zmqContext)
	{
		zmqContext = zmq_ctx_new();
		status = zmq_ctx_set(zmqContext, ZMQ_IO_THREADS, 0 < threadNumber ? threadNumber : 1) ? ERR_BAD_OPERATE : ERR_OK;
	}

	return status;
}

void MQContext::uninitialize(void)
{
	if (zmqContext)
	{
		zmq_ctx_destroy(zmqContext);
	}
}

void* MQContext::socket(const MQSocketType socketType /* = SOCKET_NONE */)
{
	return zmq_socket(zmqContext, socketType);
}

void MQContext::closesocket(void* so /* = NULL */)
{
	zmq_close(so);
}

NS_END
