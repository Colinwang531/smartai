//
//		Copyright :					@2018, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2017-06-26
//		Description :				�豸������࣬��������Ӳ���豸������ֱ�Ӽ̳��ڴ�
//
//		History:						Author										Date													Description
//											������										2017-06-26									����
//

#ifndef DEVICE_H
#define DEVICE_H

#include "predef.h"

NS_BEGIN(device, 1)

class Device
{
public:
	Device(void);
	virtual ~Device(void);

protected:
	int createNew(void);
	int destory(void);

protected:
	static int counter;
};//class Device

NS_END

#endif//DEVICE_H
