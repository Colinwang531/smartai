//
//		Copyright :					@2017, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2017-08-24
//		Description:					消息队列端口监听接口
//
//		History:						Author										Date													Description
//											王科威										2017-08-24									创建
//

#ifndef MQ_LISTENER_H
#define MQ_LISTENER_H

#include <string>
#include "predef.h"

NS_BEGIN(mq, 1)

class MQListener
{
public:
	MQListener(void);
	virtual ~MQListener(void);

	int bind(const std::string address, void* so = NULL);

protected:
	virtual int startBind(const std::string address, void* so = NULL);
};//class MQListener

NS_END

#endif//MQ_LISTENER_H
