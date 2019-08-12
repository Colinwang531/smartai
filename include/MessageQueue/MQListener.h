//
//		Copyright :					@2017, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2017-08-24
//		Description:					��Ϣ���ж˿ڼ����ӿ�
//
//		History:						Author										Date													Description
//											������										2017-08-24									����
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
