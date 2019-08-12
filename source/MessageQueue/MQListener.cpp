#include "zmq.h"
#include "error.h"
#include "MessageQueue/MQListener.h"

NS_BEGIN(mq, 1)

MQListener::MQListener()
{}

MQListener::~MQListener()
{}

int MQListener::bind(const std::string address, void* so /* = NULL */)
{
	return !address.empty() && so ? startBind(address, so) : ERR_INVALID_PARAM;
}

int MQListener::startBind(const std::string address, void* so /* = NULL */)
{
	return zmq_bind(so, address.c_str()) ? ERR_BAD_OPERATE : ERR_OK;
}

NS_END
