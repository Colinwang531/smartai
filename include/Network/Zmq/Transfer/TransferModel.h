//
//		Copyright :				@2017, ***, All Rights Reserved
//
//		Author :					������
//		E-mail :					wangkw531@icloud.com
//		Date :						2017-09-12
//		Description:				��Ϣ����ת��ģ��
//
//		History:					Author										Date												Description
//										������										2017-09-12								����
//

#ifndef TRANSFER_MODEL_H
#define TRANSFER_MODEL_H

#include "MQModel/MQModel.h"
#include "MessageQueue/MQThread.h"
using MQThread = NS(mq, 1)::MQThread;

NS_BEGIN(model, 1)

class TransferModel : public MQModel, protected MQThread
{
public:
	TransferModel(
		const unsigned short port = 60532, 
		const char* url = "inproc://WorkerProcess");
	virtual ~TransferModel(void);

protected:
	int initializeModel(MQContext& ctx) override;
	int deinitializeModel(MQContext& ctx) override;
	void process(void) override;

protected:
	void* router;
	void* dealer;
	const unsigned short routerListenPort;
	const std::string dealerListenUrl;
};//class TransferModel

NS_END

#endif//TRANSFER_MODEL_H
