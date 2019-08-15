//
//		Copyright :					@2017, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2017-12-25
//		Description:					消息队列发布模型
//
//		History:						Author									Date											Description
//											王科威									2019-07-24							创建
//


#ifndef PUBLISHER_MODEL_H
#define PUBLISHER_MODEL_H

#include "MQModel/MQModel.h"

NS_BEGIN(model, 1)

class PublisherModel : public MQModel
{
public:
	PublisherModel(const unsigned short port = 60531);
	virtual ~PublisherModel(void);

	int send(const char* data = NULL, const int dataBytes = 0) override;

protected:
	int initializeModel(void) override;
	int deinitializeModel(void) override;

protected:
	void* publisher;
	const unsigned short publishListenPort;
};//class PublisherModel

NS_END

#endif//PUBLISHER_MODEL_H
