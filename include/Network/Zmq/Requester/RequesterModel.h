//
//		Copyright :				@2017, ***, All Rights Reserved
//
//		Author :					������
//		E-mail :					wangkw531@icloud.com
//		Date :						2017-09-12
//		Description:				��Ϣ��������ģ��
//
//		History:					Author										Date												Description
//										������										2017-09-12								����
//

#ifndef REQUESTER_MODEL_H
#define REQUESTER_MODEL_H

#include "boost/thread/condition.hpp"
#include "MQModel/MQModel.h"

NS_BEGIN(model, 1)

class RequesterModel : public MQModel
{
public:
	RequesterModel(const unsigned short port = 60533);
	virtual ~RequesterModel(void);

	int send(const char* data, const int dataBytes, std::string& resp);

protected:
	int initializeModel(MQContext& ctx) override;
	int deinitializeModel(MQContext& ctx) override;

public:
	void* requester;
	const unsigned short responserListenPort;
};//class RequesterModel

NS_END

#endif//REQUESTER_MODEL_H
