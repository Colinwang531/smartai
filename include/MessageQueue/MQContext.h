//
//		Copyright :					@2017, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2017-09-14
//		Description :				��Ϣ���������Ļ���
//
//		History:						Author									Date													Description
//											������									2017-09-14									����
//

#ifndef MQ_CONTEXT_H
#define MQ_CONTEXT_H

#include "predef.h"

NS_BEGIN(mq, 1)

typedef enum tagMQSocketType_t
{
	SOCKET_NONE = 0,
	SOCKET_PUBLISHER,
	SOCKET_DEALER = 5,
	SOCKET_ROUTER
}MQSocketType;

class MQContext
{
public:
	MQContext(void);
	virtual ~MQContext(void);

public:
	int initialize(const unsigned char threadNumber = 1);
	void uninitialize(void);
	void* socket(const MQSocketType socketType = SOCKET_NONE);
	void closesocket(void* so = NULL);

protected:
	void* zmqContext;
};//class MQContext

NS_END

#endif//MQ_CONTEXT_H
