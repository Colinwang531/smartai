#include "boost/format.hpp"
#include "error.h"
#include "MessageQueue/MQListener.h"
using MQListener = NS(mq, 1)::MQListener;
#include "MessageQueue/MQSender.h"
using MQSender = NS(mq, 1)::MQSender;
#include "MQModel/PublisherServerModel.h"

NS_BEGIN(model, 1)

PublisherServerModel::PublisherServerModel() : publisher{ NULL }
{}

PublisherServerModel::~PublisherServerModel()
{}

int PublisherServerModel::start(const unsigned char threadNumber /* = 1 */, const unsigned short portNumber /* = 60531 */)
{
	//Not thread safe.
	stop();
	return startPublisher(0 < threadNumber ? threadNumber : 1, portNumber);
}

void PublisherServerModel::stop()
{
	stopPublisher();
}

int PublisherServerModel::send(const char* data /* = NULL */, const int dataBytes /* = 0 */)
{
	return data && 0 < dataBytes ? sendData(data, dataBytes) : ERR_INVALID_PARAM;
}

int PublisherServerModel::startPublisher(const unsigned char threadNumber /* = 1 */, const unsigned short portNumber /* = 60531 */)
{
	int status{ context.initialize(threadNumber) };

	if (ERR_OK == status)
	{
		publisher = context.socket(NS(mq, 1)::MQSocketType::SOCKET_PUBLISHER);

		if (publisher)
		{
			status = MQListener().bind((boost::format("tcp://*:%d") % portNumber).str(), publisher);
		}
	}

	return status;
}

void PublisherServerModel::stopPublisher()
{
	context.closesocket(publisher);
	context.uninitialize();
}

int PublisherServerModel::sendData(const char* data /* = NULL */, const int dataBytes /* = 0 */)
{
	return publisher ? MQSender().send(data, dataBytes, publisher) : ERR_BAD_OPERATE;
}

NS_END
