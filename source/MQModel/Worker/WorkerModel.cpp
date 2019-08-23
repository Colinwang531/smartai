#include "zmq.h"
#include "error.h"
#include "MessageQueue/MQConnector.h"
using MQConnector = NS(mq, 1)::MQConnector;
#include "MQModel/Worker/WorkerModel.h"

NS_BEGIN(model, 1)

WorkerModel::WorkerModel(
	const char* url/* = "inproc://WorkerProcess"*/, GetRequestMessageNotifyHandler handler/* = NULL*/)
	: MQModel(), MQThread(), dealer{ NULL }, getRequestMessageNotifyHandler{ handler }
{}

WorkerModel::~WorkerModel()
{}

int WorkerModel::initializeModel(MQContext& ctx)
{
	int status{ dealer ? ERR_EXISTED : ERR_OK };

	if (ERR_OK == status)
	{
		dealer = ctx.socket(NS(mq, 1)::MQSocketType::MQ_SOCKET_DEALER);
		if (dealer)
		{
			status = MQConnector().connect(dealer, "inproc://WorkerProcess");
			if (ERR_OK == status)
			{
				status = MQThread::start();
			}
		}
	}

	return status;
}

int WorkerModel::deinitializeModel(MQContext& ctx)
{
	MQThread::stop();

	if (dealer)
	{
		ctx.closesocket(dealer);
	}

	return ERR_OK;
}

void WorkerModel::process()
{
	zmq_pollitem_t pollitems[] = { { dealer, NULL, ZMQ_POLLIN, NULL } };

	while (!stopped)
	{
		zmq_poll(pollitems, 1, 1);
		if (pollitems[0].revents & ZMQ_POLLIN)
		{
			zmq_msg_t identity, delimiter, request, reply;
			zmq_msg_init(&identity);
			zmq_msg_init(&delimiter);
			zmq_msg_init(&request);

			const int identityBytes{ zmq_msg_recv(&identity, dealer, ZMQ_DONTWAIT | ZMQ_MORE) };
			const int delimiterBytes{ zmq_msg_recv(&delimiter, dealer, ZMQ_DONTWAIT | ZMQ_MORE) };
			const int requestBytes{ zmq_msg_recv(&request, dealer, 0) };
			const char* requestData{ (const char*)zmq_msg_data(&request) };

			if (getRequestMessageNotifyHandler)
			{
				char* replyResult{ getRequestMessageNotifyHandler(requestData, requestBytes) };

				zmq_msg_init_data(&reply, (void*)replyResult, (int)strlen(replyResult), NULL, NULL);
				zmq_msg_send(&identity, dealer, ZMQ_DONTWAIT | ZMQ_SNDMORE);
				zmq_msg_send(&delimiter, dealer, ZMQ_DONTWAIT | ZMQ_SNDMORE);
				zmq_msg_send(&reply, dealer, ZMQ_DONTWAIT);
				zmq_msg_close(&reply);
			}

			zmq_msg_close(&identity);
			zmq_msg_close(&delimiter);
			zmq_msg_close(&request);
		}
	}
}

NS_END