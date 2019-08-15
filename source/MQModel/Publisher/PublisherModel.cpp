#include "boost/format.hpp"
#include "error.h"
#include "MessageQueue/MQListener.h"
using MQListener = NS(mq, 1)::MQListener;
#include "MessageQueue/MQSender.h"
using MQSender = NS(mq, 1)::MQSender;
#include "MQModel/Publisher/PublisherModel.h"

NS_BEGIN(model, 1)

PublisherModel::PublisherModel(const unsigned short port /* = 60531 */)
	: MQModel(), publisher{ NULL }, publishListenPort{ port }
{}

PublisherModel::~PublisherModel()
{}

int PublisherModel::initializeModel()
{
	int status{ MQModel::initializeModel() };

	if (ERR_OK == status)
	{
		publisher = ctx.socket(NS(mq, 1)::MQSocketType::MQ_SOCKET_PUBLISHER);

		if (publisher)
		{
			status = MQListener().bind((boost::format("tcp://*:%d") % publishListenPort).str(), publisher);
		}
	}

	return status;
}

int PublisherModel::deinitializeModel()
{
	if (publisher)
	{
		ctx.closesocket(publisher);
		MQModel::deinitializeModel();
	}

	return ERR_OK;
}

int PublisherModel::send(const char* data /* = NULL */, const int dataBytes /* = 0 */)
{
	int status{ MQModel::send(data, dataBytes) };

	if (ERR_OK == status)
	{
		status = publisher ? MQSender().send(data, dataBytes, publisher) : ERR_BAD_OPERATE;
	}

	return status;
}

NS_END
