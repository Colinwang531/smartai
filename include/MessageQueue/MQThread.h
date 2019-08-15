//
//		Copyright :						@2017, ***, All Rights Reserved
//
//		Author :							王科威
//		E-mail :							wangkw531@hotmail.com
//		Date :								2017-08-29
//		Description:						工作者线程
//
//		History:							Author										Date												Description
//												王科威										2017-08-29								创建
//

#ifndef MQ_THREAD_H
#define MQ_THREAD_H

#include "predef.h"

NS_BEGIN(mq, 1)

class MQThread 
{
public:
	MQThread(void);
	virtual ~MQThread(void);

	int startThread(void* handler = NULL, void* ctx = NULL);
	void stopThread(void);

protected:
	void* threadID;
};//class MQThread

NS_END

#endif//MQ_THREAD_H
