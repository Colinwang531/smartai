//
//		Copyright :					@2017, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2017-08-24
//		Description:					消息队列端口连接接口
//
//		History:						Author										Date													Description
//											王科威										2017-08-24									创建
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
