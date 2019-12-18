#include "boost/format.hpp"
#include "zmq.h"
#include "error.h"
#include "MessageQueue/MQConnector.h"
using MQConnector = NS(mq, 1)::MQConnector;
#include "MessageQueue/MQSender.h"
using MQSender = NS(mq, 1)::MQSender;
#include "MessageQueue/MQReceiver.h"
using MQReceiver = NS(mq, 1)::MQReceiver;
#include "MQModel/Requester/RequesterModel.h"

NS_BEGIN(model, 1)

RequesterModel::RequesterModel(const unsigned short port /* = 60533 */)
	: MQModel(), requester{ NULL }, responserListenPort{ port }
{}

RequesterModel::~RequesterModel()
{}

int RequesterModel::initializeModel(MQContext& ctx)
{
	int status{ ERR_BAD_ALLOC };
	requester = ctx.socket(NS(mq, 1)::MQSocketType::MQ_SOCKET_DEALER);

	if (requester)
	{
		status = MQConnector().connect(requester, (boost::format("tcp://127.0.0.1:%d") % responserListenPort).str().c_str());
	}

	return status;
}

int RequesterModel::deinitializeModel(MQContext& ctx)
{
	int status{ ERR_INVALID_PARAM };

	if (requester)
	{
		ctx.closesocket(requester);
		status = ERR_OK;
	}

	return status;
}

int RequesterModel::send(const char* data, const int dataBytes, std::string& resp)
{
	int status{ ERR_INVALID_PARAM };

	if (data && 0 < dataBytes)
	{
		status = requester ? MQSender().send(data, dataBytes, requester) : ERR_BAD_OPERATE;

		//if (ERR_OK == status)
		//{
		//	zmq_pollitem_t pollitems[] = { { requester, NULL, ZMQ_POLLIN, NULL } };
		//	zmq_poll(pollitems, 1, 30000);
		//	if (pollitems[0].revents & ZMQ_POLLIN)
		//	{
		//		status = MQReceiver().receive(requester, resp);
		//	}
		//	else
		//	{
		//		status = ERR_BAD_OPERATE;
		//	}
		//}
	}

	return status;
}

NS_END
