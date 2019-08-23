#include "MessageQueue/MQContext.h"
#include "MQModel/MQModel.h"

NS_BEGIN(model, 1)

MQModel::MQModel()
{}

MQModel::~MQModel()
{}

int MQModel::start(MQContext& ctx)
{
	return initializeModel(ctx);
}

void MQModel::stop(MQContext& ctx)
{
	deinitializeModel(ctx);
}

NS_END
