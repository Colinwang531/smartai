//
//		Copyright :				@2017, ***, All Rights Reserved
//
//		Author :					������
//		E-mail :					wangkw531@icloud.com
//		Date :						2017-09-12
//		Description:				��Ϣ����ģ�ͳ�����࣬����ģ�Ͷ��̳��ڴ�
//
//		History:					Author										Date												Description
//										������										2017-09-12								����
//

#ifndef MQ_MODEL_H
#define MQ_MODEL_H

#include "MessageQueue/MQContext.h"
using MQContext = NS(mq, 1)::MQContext;

NS_BEGIN(model, 1)

class MQModel
{
public:
	MQModel(void);
	virtual ~MQModel(void);

public:
	int start(MQContext& ctx);
	void stop(MQContext& ctx);

protected:
	virtual int initializeModel(MQContext& ctx) = 0;
	virtual int deinitializeModel(MQContext& ctx) = 0;
};//class MQModel

NS_END

#endif//MQ_MODEL_H
