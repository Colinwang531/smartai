//
//		Copyright :					@2017, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2017-12-25
//		Description:					��Ϣ���з���ģ��
//
//		History:						Author									Date											Description
//											������									2019-07-24							����
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
