#include "zmq.h"
#include "MessageQueue/MQListener.h"
#include "MessageQueue/MQTransfer.h"
#include "MessageQueue/MQContext.h"
#include "MQModel/Transfer/TransferModel.h"

NS_BEGIN(mq, 1)

TransferModel::TransferModel()
{}

TransferModel::~TransferModel()
{}

Int32 TransferModel::startModel(MQContext& ctx, const std::string& rAddr, const std::string& sAddr)
{
	Int32 status{ !ctx.emptyContext() && !rAddr.empty() && !sAddr.empty() ? ERR_OK : ERR_INVALID_PARAM };

	if (ERR_OK == status)
	{
		if (ERR_OK == listeningRouter(ctx, rAddr) && ERR_OK == listeningDealer(ctx, sAddr))
		{
			status = transferDataThread.startThread(&TransferModel::workerThreadHandler, this);
		}
		else
		{
			status = ERR_BAD_OPERATE;
		}
	}

	return status;
}

void TransferModel::stopModel(MQContext& ctx)
{
	if (!ctx.emptyContext())
	{
		transferDataThread.stopThread();
		ctx.destroySocket(routerSocket);
		ctx.destroySocket(dealerSocket);
	}
}

Int32 TransferModel::listeningRouter(MQContext& ctx, const std::string& rAddr)
{
	Int32 status{ ERR_BAD_OPERATE };

	if (!routerSocket)
	{
		routerSocket = ctx.createSocket(MQ_SOCKET_TYPE_ROUTER);
		if (routerSocket)
		{
			status = MQListener().startListen(rAddr, routerSocket);
		}
	}

	return status;
}

Int32 TransferModel::listeningDealer(MQContext& ctx, const std::string& sAddr)
{
	Int32 status{ ERR_BAD_OPERATE };

	if (!dealerSocket)
	{
		dealerSocket = ctx.createSocket(MQ_SOCKET_TYPE_DEALER);
		if (dealerSocket)
		{
			status = MQListener().startListen(sAddr, dealerSocket);
		}
	}

	return status;
}

void TransferModel::workerThreadHandler(HANDLE ctx /* = nullptr */)
{
	TransferModel* tempTransferModel{ reinterpret_cast<TransferModel*>(ctx) };
	zmq_pollitem_t tempPollItems[] = {
		{ tempTransferModel->routerSocket, NULL, ZMQ_POLLIN, NULL }, { tempTransferModel->dealerSocket, NULL, ZMQ_POLLIN, NULL } };
	int tempPollItemNumber{ tempTransferModel->routerSocket && tempTransferModel->dealerSocket ? 2 : 1 };

	while (!tempTransferModel->transferDataThread.Stopped())
	{
		zmq_poll(tempPollItems, tempPollItemNumber, 1);
		if (tempPollItems[0].revents & ZMQ_POLLIN)
		{
			MQTransfer().transferData(tempTransferModel->routerSocket, tempTransferModel->dealerSocket);
		}
		else if (tempPollItems[1].revents & ZMQ_POLLIN)
		{
			MQTransfer().transferData(tempTransferModel->dealerSocket, tempTransferModel->routerSocket);
		}
	}
}

NS_END