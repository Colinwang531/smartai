#include "zmq.h"
#include "error.h"
#include "MessageQueue/MQReceiver.h"

NS_BEGIN(mq, 1)

MQReceiver::MQReceiver()
{}

MQReceiver::~MQReceiver()
{}

int MQReceiver::receive(void* so, std::string& data)
{
	int status{ ERR_INVALID_PARAM };

	if (so)
	{
		zmq_msg_t msg;
		zmq_msg_init(&msg);
		int anymore{ ZMQ_MORE };

		while (anymore)
		{
			zmq_msg_recv(&msg, so, ZMQ_DONTWAIT);
			anymore = zmq_msg_more(&msg);
		}

		data.assign(reinterpret_cast<const char*>(zmq_msg_data(&msg)), zmq_msg_size(&msg));
		zmq_msg_close(&msg);
		status = ERR_OK;
	}
	
	return status;
}

NS_END
