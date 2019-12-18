#include "zmq.h"
#include "error.h"
#include "MessageQueue/MQTransfer.h"

NS_BEGIN(mq, 1)

MQTransfer::MQTransfer()
{}

MQTransfer::~MQTransfer()
{}

int MQTransfer::transfer(void* from /* = NULL */, void* to /* = NULL */)
{
	return from && to ? transferData(from, to) : ERR_INVALID_PARAM;
}

int MQTransfer::transferData(void* from /* = NULL */, void* to /* = NULL */)
{
	int tempSendDataBytes = 0, flag{ ZMQ_DONTWAIT };
	zmq_msg_t recvMsg;
	zmq_msg_init(&recvMsg);
	int recvDataBytes{ zmq_msg_recv(&recvMsg, from, flag) };

	if (zmq_msg_more(&recvMsg))
	{
		flag |= ZMQ_SNDMORE;
	}

	zmq_msg_t sendMsg;
	zmq_msg_init(&sendMsg);
	zmq_msg_copy(&sendMsg, &recvMsg);
	int sendDataBytes = zmq_msg_send(&sendMsg, to, flag);

	zmq_msg_close(&recvMsg);
	zmq_msg_close(&sendMsg);

	return ERR_OK;
}

NS_END
