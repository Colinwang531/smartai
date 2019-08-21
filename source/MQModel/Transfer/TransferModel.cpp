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

TransferModel::TransferModel(const unsigned short port /* = 60532 */)
	: MQModel(), router{ NULL }, dealer{ NULL }, routerListenPort{ port }, workerSocket{ NULL }
{
	zmq_msg_init(&replyIdentity);
	zmq_msg_init(&replayDelimiter);
}

TransferModel::~TransferModel()
{}

int TransferModel::initializeModel()
{
	int status{ MQModel::initializeModel() };

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
					//Run worker thread processing after model has been finished initializing.
					condition.notify_one();

					for (int i = 0; i != WORKER_THREAD_NUM; ++i)
					{
						workerThread[i] = zmq_threadstart(&TransferModel::workerThreadHandler, this);
					}
				}
			}
		}
	}

	return status;
}

int TransferModel::deinitializeModel()
{
	if (router)
	{
		ctx.closesocket(router);
	}

	if (dealer)
	{
		ctx.closesocket(dealer);
	}

	return MQModel::deinitializeModel();
}

void TransferModel::workerThreadProcess()
{
	boost::unique_lock<boost::mutex> lock{ mtx };
	condition.wait(lock);
	zmq_pollitem_t tempPollItems[] = {
		{ router, NULL, ZMQ_POLLIN, NULL }, { dealer, NULL, ZMQ_POLLIN, NULL } };

	while (!stopped)
	{
		zmq_poll(tempPollItems, 2, 1);
		if (tempPollItems[0].revents & ZMQ_POLLIN)
		{
			MQTransfer().transfer(router, dealer);
		}
		else if (tempPollItems[1].revents & ZMQ_POLLIN)
		{
			MQTransfer().transfer(dealer, router);
		}
	}
}

int TransferModel::sendReply(
	const char* msg /* = NULL */, const int msgBytes /* = 0 */)
{
	if (workerSocket)
	{
//		const char* delimiter = "";
		zmq_msg_t /*identityMsg, delimiterMsg,*/ requestMsg;
// 		zmq_msg_init_data(&identityMsg, (void*)identity, (int)strlen(identity), NULL, NULL);
// 		zmq_msg_init_data(&delimiterMsg, (void*)delimiter, 0, NULL, NULL);
		zmq_msg_init_data(&requestMsg, (void*)msg, msgBytes, NULL, NULL);
		zmq_msg_send(&replyIdentity, workerSocket, ZMQ_DONTWAIT | ZMQ_SNDMORE);
		zmq_msg_send(&replayDelimiter, workerSocket, ZMQ_DONTWAIT | ZMQ_SNDMORE);
		zmq_msg_send(&requestMsg, workerSocket, ZMQ_DONTWAIT);
// 		zmq_msg_close(&identityMsg);
// 		zmq_msg_close(&delimiterMsg);
		zmq_msg_close(&requestMsg);
	}

	return ERR_OK;
}

void TransferModel::workerThreadHandler(void* ctx /* = NULL */)
{
	TransferModel* transfer{ reinterpret_cast<TransferModel*>(ctx) };
	if (transfer)
	{
		transfer->workerSocket =  transfer->ctx.socket(NS(mq, 1)::MQSocketType::MQ_SOCKET_DEALER);
		if (transfer->workerSocket)
		{
			int ret = zmq_connect(transfer->workerSocket, "inproc://WorkerProcess");
			zmq_pollitem_t tempPollItems[] = { { transfer->workerSocket, NULL, ZMQ_POLLIN, NULL } };

			while (!ret && !transfer->stopped)
			{
				zmq_poll(tempPollItems, 1, 1);
				if (tempPollItems[0].revents & ZMQ_POLLIN)
				{
					zmq_msg_t identityMsg, delimiterMsg, requestMsg;
					zmq_msg_init(&identityMsg);
					zmq_msg_init(&delimiterMsg);
					zmq_msg_init(&requestMsg);

					int identityBytes{ zmq_msg_recv(&identityMsg, transfer->workerSocket, ZMQ_DONTWAIT | ZMQ_MORE) };
					int delimiterBytes{ zmq_msg_recv(&delimiterMsg, transfer->workerSocket, ZMQ_DONTWAIT | ZMQ_MORE) };
					int requestBytes{ zmq_msg_recv(&requestMsg, transfer->workerSocket, 0) };

					const char* identity{ (const char*)zmq_msg_data(&identityMsg) };
					const char* delimiter{ (const char*)zmq_msg_data(&delimiterMsg) };
					const char* request{ (const char*)zmq_msg_data(&requestMsg) };

					zmq_msg_copy(&transfer->replyIdentity, &identityMsg);
					zmq_msg_copy(&transfer->replayDelimiter, &delimiterMsg);

					transfer->gotMessageData(request, requestBytes);

					zmq_msg_close(&identityMsg);
					zmq_msg_close(&delimiterMsg);
					zmq_msg_close(&requestMsg);
				}
			}
		}
	}
}

NS_END