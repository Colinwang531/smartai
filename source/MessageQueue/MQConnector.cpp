#include "zmq.h"
#include "error.h"
#include "MessageQueue/MQConnector.h"

NS_BEGIN(mq, 1)

MQConnector::MQConnector()
{}

MQConnector::~MQConnector()
{}

int MQConnector::connect(void* so /* = NULL */, const char* address /* = NULL */)
{
	int status{ ERR_INVALID_PARAM };

	if (so && address)
	{
		status = !zmq_connect(so, address) ? ERR_OK : ERR_BAD_OPERATE;
	}

	return status;
}

NS_END
