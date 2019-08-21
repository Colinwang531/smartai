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

#include "predef.h"

NS_BEGIN(model, 1)

class MQModel
{
public:
	MQModel(void);
	virtual ~MQModel(void);

public:
	int start(void);
	void stop(void);

protected:
	virtual int initializeModel(void) = 0;
	virtual int deinitializeModel(void) = 0;
};//class TransferModel

NS_END

#endif//MQ_MODEL_H
