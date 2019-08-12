//
//		Copyright :					@2017, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2017-12-25
//		Description:					ZMQ发布模式服务端
//
//		History:						Author									Date											Description
//											王科威									2019-07-24							创建
//


#ifndef PUBLISHER_SERVER_MODEL_H
#define PUBLISHER_SERVER_MODEL_H

#include "MessageQueue/MQContext.h"
using MQContext = NS(mq, 1)::MQContext;

NS_BEGIN(model, 1)

class PublisherServerModel
{
public:
	PublisherServerModel(void);
	virtual ~PublisherServerModel(void);

	int start(const unsigned char threadNumber = 1, const unsigned short portNumber = 60531);
	void stop(void);
	int send(const char* data = NULL, const int dataBytes = 0);

protected:
	virtual int startPublisher(const unsigned char threadNumber = 1, const unsigned short portNumber = 60531);
	virtual void stopPublisher(void);
	virtual int sendData(const char* data = NULL, const int dataBytes = 0);

protected:
	MQContext context;
	void* publisher;
};//class PublisherServerModel

NS_END

#endif//PUBLISHER_SERVER_MODEL_H
