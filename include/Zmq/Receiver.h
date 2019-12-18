//
//		Copyright :				@2017, ***, All Rights Reserved
//
//		Author :					王科威
//		E-mail :					wangkw531@hotmail.com
//		Date :						2017-09-14
//		Description :			数据接收接口
//
//		History:					Author									Date											Description
//										王科威									2017-09-14							创建
//

#ifndef MQ_RECEIVER_H
#define MQ_RECEIVER_H

#include <string>
#include "predef.h"

NS_BEGIN(mq, 1)

class MQReceiver
{
public:
	MQReceiver(void);
	virtual ~MQReceiver(void);

public:
	virtual int receive(void* so, std::string& data);
};//class MQReceiver

NS_END

#endif//MQ_RECEIVER_H
