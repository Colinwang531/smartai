#include "zmq.h"
#include "error.h"
#include "MQModel/MQModel.h"

NS_BEGIN(model, 1)

MQModel::MQModel() : stopped{ true }
{}

MQModel::~MQModel()
{}

int MQModel::start(const unsigned char threadNumber /* = 1 */)
{
	int status{ ctx.initialize(0 < threadNumber ? threadNumber : 1) };

	if (ERR_OK == status)
	{
		status = initializeModel();
		stopped = false;
	}

	return status;
}

void MQModel::stop()
{
	stopped = true;
	deinitializeModel();
	ctx.deinitialize();
}

int MQModel::send(const char* data /* = NULL */, const int dataBytes /* = 0 */)
{
	return data && 0 < dataBytes ? ERR_OK : ERR_INVALID_PARAM;
}

int MQModel::initializeModel()
{
	return workerThread.startThread(&MQModel::workerThreadHandler, this);
}

int MQModel::deinitializeModel()
{
	workerThread.stopThread();
	return ERR_OK;
}

void MQModel::workerThreadHandler(void* ctx /* = nullptr */)
{
	MQModel* model{ reinterpret_cast<MQModel*>(ctx) };

	if (model)
	{
		model->workerThreadProcess();
	}
}

void MQModel::workerThreadProcess()
{}

NS_END
