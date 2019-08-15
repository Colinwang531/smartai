//
//		Copyright :				@2017, ***, All Rights Reserved
//
//		Author :					������
//		E-mail :					wangkw531@icloud.com
//		Date :						2017-09-12
//		Description:				��Ϣ����·��ģ��
//
//		History:					Author										Date												Description
//										������										2017-09-12								����
//

#ifndef ROUTER_MODEL_H
#define ROUTER_MODEL_H

#include "boost/thread/condition.hpp"
#include "MQModel/MQModel.h"

NS_BEGIN(model, 1)

class RouterModel : public MQModel
{
public:
	RouterModel(const unsigned short port = 60532);
	virtual ~RouterModel(void);

	int send(const char* data = NULL, const int dataBytes = 0) override;

protected:
	int initializeModel(void) override;
	int deinitializeModel(void) override;
	void workerThreadProcess(void) override;
	virtual void gotMessageData(const char* data = NULL, const int dataBytes = 0);

private:
	void* router;
	const unsigned short routerListenPort;
	boost::mutex mtx;
	boost::condition condition;
};//class RouterModel

NS_END

#endif//ROUTER_MODEL_H
