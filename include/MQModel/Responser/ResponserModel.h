//
//		Copyright :				@2017, ***, All Rights Reserved
//
//		Author :					������
//		E-mail :					wangkw531@icloud.com
//		Date :						2017-09-12
//		Description:				��Ϣ����Ӧ��ģ��
//
//		History:					Author										Date												Description
//										������										2017-09-12								����
//

#ifndef RESPONSER_MODEL_H
#define RESPONSER_MODEL_H

#include "boost/function.hpp"
#include "boost/thread/condition.hpp"
#include "MQModel/MQModel.h"
#include "MessageQueue/MQThread.h"
using MQThread = NS(mq, 1)::MQThread;

NS_BEGIN(model, 1)

typedef boost::function<int(const char*, const int, const char*, const int)> GetRequestMessageNotifyHandler;

class ResponserModel : public MQModel, protected MQThread
{
public:
	ResponserModel(
		const unsigned short port = 60533, GetRequestMessageNotifyHandler handler = NULL);
	virtual ~ResponserModel(void);

protected:
	int initializeModel(MQContext& ctx) override;
	int deinitializeModel(MQContext& ctx) override;
	void process(void) override;

public:
	void* responser;
	const unsigned short reponserListenPort;
	GetRequestMessageNotifyHandler getRequestMessageNotifyHandler;
};//class ResponserModel

NS_END

#endif//RESPONSER_MODEL_H
