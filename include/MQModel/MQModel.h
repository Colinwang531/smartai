//
//		Copyright :				@2017, ***, All Rights Reserved
//
//		Author :					王科威
//		E-mail :					wangkw531@icloud.com
//		Date :						2017-09-12
//		Description:				消息队列模型抽象基类，所有模型都继承于此
//
//		History:					Author										Date												Description
//										王科威										2017-09-12								创建
//

#ifndef MQ_MODEL_H
#define MQ_MODEL_H

#include "MessageQueue/MQContext.h"
using MQContext = NS(mq, 1)::MQContext;
#include "MessageQueue/MQThread.h"
using MQThread = NS(mq, 1)::MQThread;

NS_BEGIN(model, 1)

class MQModel
{
public:
	MQModel(void);
	virtual ~MQModel(void);

public:
	int start(const unsigned char threadNumber = 1);
	void stop(void);
	virtual int send(const char* data = NULL, const int dataBytes = 0);

protected:
	virtual int initializeModel(void);
	virtual int deinitializeModel(void);
	virtual void workerThreadProcess(void);

private:
	static void workerThreadHandler(void* ctx = NULL);

protected:
	MQContext ctx;
	MQThread workerThread;
	bool stopped;
};//class TransferModel

NS_END

#endif//MQ_MODEL_H
