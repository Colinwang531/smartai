#include "boost/format.hpp"
#include "zmq.h"
#include "error.h"
#include "MessageQueue/MQListener.h"
using MQListener = NS(mq, 1)::MQListener;
#include "MessageQueue/MQTransfer.h"
using MQTransfer = NS(mq, 1)::MQTransfer;
#include "MessageQueue/MQReceiver.h"
using MQReceiver = NS(mq, 1)::MQReceiver;
#include "MessageQueue/MQSender.h"
using MQSender = NS(mq, 1)::MQSender;
#include "MQModel/Transfer/TransferModel.h"

NS_BEGIN(model, 1)

TransferModel::TransferModel(
	const unsigned short port /* = 60532 */, const char* url /* = "inproc */)
	: MQModel(), MQThread(), router{ NULL }, dealer{ NULL }, routerListenPort{ port }, dealerListenUrl{ url }
{}

TransferModel::~TransferModel()
{}

int TransferModel::initializeModel(MQContext& ctx)
{
	int status{ router || dealer ? ERR_EXISTED : ERR_OK };

	if (ERR_OK == status)
	{
		router = ctx.socket(NS(mq, 1)::MQSocketType::MQ_SOCKET_ROUTER);
		if (router)
		{
			status = MQListener().bind((boost::format("tcp://*:%d") % routerListenPort).str(), router);
			if (ERR_OK == status)
			{
				dealer = ctx.socket(NS(mq, 1)::MQSocketType::MQ_SOCKET_DEALER);
				if (dealer)
				{
					status = MQListener().bind("inproc://WorkerProcess", dealer);

					if (ERR_OK == status)
					{
						status = MQThread::start();
					}
				}
			}
		}
	}

	return status;
}

int TransferModel::deinitializeModel(MQContext& ctx)
{
	MQThread::stop();

	if (router)
	{
		ctx.closesocket(router);
	}

	if (dealer)
	{
		ctx.closesocket(dealer);
	}

	return ERR_OK;
}

void TransferModel::process()
{
	zmq_pollitem_t pollitems[] = {
		{ router, NULL, ZMQ_POLLIN, NULL }, { dealer, NULL, ZMQ_POLLIN, NULL } };

	while (!stopped)
	{
		zmq_poll(pollitems, 2, 1);
		if (pollitems[0].revents & ZMQ_POLLIN)
		{
			MQTransfer().transfer(router, dealer);
		}
		else if (pollitems[1].revents & ZMQ_POLLIN)
		{
			MQTransfer().transfer(dealer, router);
		}
	}
}

NS_END