//
//		Copyright :				@2017, ***, All Rights Reserved
//
//		Author :					王科威
//		E-mail :					wangkw531@icloud.com
//		Date :						2017-09-12
//		Description:				消息队列工作者模型
//
//		History:					Author										Date												Description
//										王科威										2017-09-12								创建
//

#ifndef WORKER_MODEL_H
#define WORKER_MODEL_H

#include "boost/function.hpp"
#include "MQModel/MQModel.h"
#include "MessageQueue/MQThread.h"
using MQThread = NS(mq, 1)::MQThread;

NS_BEGIN(model, 1)

typedef boost::function<int(const char*, const int, const char*, const int)> GetRequestMessageNotifyHandler;

class WorkerModel : public MQModel, protected MQThread
{
public:
	WorkerModel(
		const char* url = "inproc://WorkerProcess", GetRequestMessageNotifyHandler handler = NULL);
	virtual ~WorkerModel(void);

protected:
	int initializeModel(MQContext& ctx) override;
	int deinitializeModel(MQContext& ctx) override;
	void process(void) override;

private:
	void* dealer;
	const std::string dealerListenUrl;
	GetRequestMessageNotifyHandler getRequestMessageNotifyHandler;
};//class TerminatorModel

NS_END

#endif//WORKER_MODEL_H
