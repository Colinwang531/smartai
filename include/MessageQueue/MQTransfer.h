//
//		Copyright :					@2017, HHJT, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@hotmail.com
//		Date :							2017-12-06
//		Description :				����ת���ӿ�
//
//		History:						Author									Date												Description
//											������									2017-12-06								����
//

#ifndef MQ_TRANSFER_H
#define MQ_TRANSFER_H

#include "predef.h"

NS_BEGIN(mq, 1)

class MQTransfer
{
public:
	MQTransfer(void);
	virtual ~MQTransfer(void);

	int transfer(void* from = NULL, void* to = NULL);

protected:
	virtual int transferData(void* from = NULL, void* to = NULL);
};//class MQTransfer

NS_END

#endif//MQ_TRANSFER_H
