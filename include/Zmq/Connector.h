//
//		Copyright :					@2017, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2017-08-24
//		Description:					��Ϣ���ж˿����ӽӿ�
//
//		History:						Author										Date													Description
//											������										2017-08-24									����
//

#ifndef MQ_CONNECTOR_H
#define MQ_CONNECTOR_H

#include "predef.h"

NS_BEGIN(mq, 1)

class MQConnector
{
public:
	MQConnector(void);
	virtual ~MQConnector(void);

	int connect(void* so = NULL, const char* address = NULL);
};//class MQConnector

NS_END

#endif//MQ_CONNECTOR_H
