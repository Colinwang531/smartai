//
//		Copyright :					@2017, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2017-09-14
//		Description :				消息队列上下文基类
//
//		History:						Author									Date													Description
//											王科威									2017-09-14									创建
//

#ifndef MQ_CONTEXT_H
#define MQ_CONTEXT_H

#include "predef.h"

NS_BEGIN(mq, 1)

typedef enum tagMQSocketType_t
{
	MQ_SOCKET_NONE = 0,
	MQ_SOCKET_PUBLISHER,
	MQ_SOCKET_REQ = 3,
	MQ_SOCKET_REP,
	MQ_SOCKET_DEALER,
	MQ_SOCKET_ROUTER,
}MQSocketType;

class MQContext
{
public:
	MQContext(void);
	virtual ~MQContext(void);

public:
	int initialize(const unsigned char threadNumber = 1);
	void deinitialize(void);
	void* socket(const MQSocketType type = MQ_SOCKET_NONE);
	void closesocket(void* so = NULL);

protected:
	//Context instance must be created only once.
	void* ctx;
};//class MQContext

NS_END

#endif//MQ_CONTEXT_H
