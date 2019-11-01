#include "boost/format.hpp"
#include "zmq.h"
#include "error.h"
#include "MessageQueue/MQListener.h"
using MQListener = NS(mq, 1)::MQListener;
#include "MessageQueue/MQSender.h"
using MQSender = NS(mq, 1)::MQSender;
#include "MessageQueue/MQReceiver.h"
using MQReceiver = NS(mq, 1)::MQReceiver;
#include "MQModel/Responser/ResponserModel.h"

NS_BEGIN(model, 1)

ResponserModel::ResponserModel(
	const unsigned short port /* = 60533 */, GetRequestMessageNotifyHandler handler /* = NULL */)
	: MQModel(), responser{ NULL }, reponserListenPort{ port }, getRequestMessageNotifyHandler{ handler }
{}

ResponserModel::~ResponserModel()
{}

int ResponserModel::initializeModel(MQContext& ctx)
{
	int status{ ERR_OK };
	responser = ctx.socket(NS(mq, 1)::MQSocketType::MQ_SOCKET_DEALER);

	if (responser)
	{
		status = MQListener().bind((boost::format("tcp://127.0.0.1:%d") % reponserListenPort).str(), responser);
		if (ERR_OK == status)
		{
			status = MQThread::start();
		}
	}

	return status;
}

int ResponserModel::deinitializeModel(MQContext& ctx)
{
	MQThread::stop();

	if (responser)
	{
		ctx.closesocket(responser);
	}

	return ERR_OK;
}

void ResponserModel::process()
{
	zmq_pollitem_t pollitems[] = { { responser, NULL, ZMQ_POLLIN, NULL } };

	while (!stopped)
	{
		zmq_poll(pollitems, 1, 1);
		if (pollitems[0].revents & ZMQ_POLLIN)
		{
			std::string msg;
			int status{ MQReceiver().receive(responser, msg) };

			if (ERR_OK == status && !msg.empty()) 
			{
				const int responseDataBytes{ 3 * 1024 * 1024 };
				char* responseData{ new(std::nothrow) char[responseDataBytes] };
				if (responseData && getRequestMessageNotifyHandler)
				{
					int respBytes{
						getRequestMessageNotifyHandler(msg.c_str(), static_cast<const int>(msg.length()), responseData, responseDataBytes) };
//					MQSender().send(responseData, respBytes, responser);
				}
				boost::checked_array_delete(responseData);
			}
		}
	}
}

NS_END
