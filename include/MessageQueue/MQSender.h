//
//		Copyright :					@2017, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2017-09-14
//		Description :				���ݷ��ͽӿ�
//
//		History:						Author									Date												Description
//											������									2017-09-14								����
//

#ifndef MQ_SENDER_H
#define MQ_SENDER_H

#include "predef.h"

NS_BEGIN(mq, 1)

class MQSender
{
public:
	MQSender(void);
	virtual ~MQSender(void);

	int send(const char* data = NULL, const int dataBytes = 0, void* so = NULL);

protected:
	virtual int sendData(
		const char* data = NULL, const int dataBytes = 0, void* so = NULL, const bool more = false);
};//class MQSender

NS_END

#endif//MQ_SENDER_H
