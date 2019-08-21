#include "zmq.h"
#include "error.h"
#include "MQModel/MQModel.h"

NS_BEGIN(model, 1)

MQModel::MQModel()
{}

MQModel::~MQModel()
{}

int MQModel::start()
{
	return initializeModel();
}

void MQModel::stop()
{
	deinitializeModel();
}

NS_END
