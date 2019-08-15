#include "boost/format.hpp"
#include "zmq.h"
#include "error.h"
#include "MessageQueue/MQListener.h"
using MQListener = NS(mq, 1)::MQListener;
#include "MessageQueue/MQSender.h"
using MQSender = NS(mq, 1)::MQSender;
#include "MessageQueue/MQReceiver.h"
using MQReceiver = NS(mq, 1)::MQReceiver;
#include "MQModel/Router/RouterModel.h"

NS_BEGIN(model, 1)

RouterModel::RouterModel(const unsigned short port /* = 60532 */)
	: MQModel(), router{ NULL }, routerListenPort{ port }
{}

RouterModel::~RouterModel()
{}

int RouterModel::initializeModel()
{
	int status{ MQModel::initializeModel() };

	if (ERR_OK == status)
	{
		router = ctx.socket(NS(mq, 1)::MQSocketType::MQ_SOCKET_ROUTER);

		if (router)
		{
			status = MQListener().bind((boost::format("tcp://*:%d") % routerListenPort).str(), router);
			//Run worker thread processing after model has been finished initializing.
			condition.notify_one();
		}
	}

	return status;
}

int RouterModel::deinitializeModel()
{
	if (router)
	{
		ctx.closesocket(router);
		MQModel::deinitializeModel();
	}

	return ERR_OK;
}

int RouterModel::send(const char* data /* = NULL */, const int dataBytes /* = 0 */)
{
	int status{ MQModel::send(data, dataBytes) };

	if (ERR_OK == status)
	{
		status = router ? MQSender().send(data, dataBytes, router) : ERR_BAD_OPERATE;
	}

	return status;
}

void RouterModel::workerThreadProcess()
{
	boost::unique_lock<boost::mutex> lock{ mtx };
	condition.wait(lock);
	zmq_pollitem_t pollitems[] = { { router, NULL, ZMQ_POLLIN, NULL } };

	while (!stopped)
	{
		zmq_poll(pollitems, 1, 1);
		if (pollitems[0].revents & ZMQ_POLLIN)
		{
			std::string msg;
			int status{ MQReceiver().receive(router, msg) };

			if (ERR_OK == status && !msg.empty()) 
			{
				gotMessageData(msg.c_str(), static_cast<const int>(msg.length()));
			}
		}
	}
}

void RouterModel::gotMessageData(const char* data /* = NULL */, const int dataBytes /* = 0 */)
{}

NS_END
