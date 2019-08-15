#include "zmq.h"
#include "error.h"
#include "MessageQueue/MQContext.h"

NS_BEGIN(mq, 1)

void* MQContext::ctx = NULL;

MQContext::MQContext()
{}

MQContext::~MQContext()
{}

int MQContext::initialize(const unsigned char threadNumber /*= 1*/)
{
	int status{ ERR_OK };

	if (!ctx)
	{
		ctx = zmq_ctx_new();
		status = zmq_ctx_set(ctx, ZMQ_IO_THREADS, 0 < threadNumber ? threadNumber : 1) ? ERR_BAD_OPERATE : ERR_OK;
	}

	return status;
}

void MQContext::deinitialize(void)
{
	if (ctx)
	{
		zmq_ctx_destroy(ctx);
	}
}

void* MQContext::socket(const MQSocketType socketType /* = SOCKET_NONE */)
{
	return zmq_socket(ctx, socketType);
}

void MQContext::closesocket(void* so /* = NULL */)
{
	zmq_close(so);
}

NS_END
