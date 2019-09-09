//
//		Copyright :					@2017, HHJT, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@hotmail.com
//		Date :							2018-05-02
//		Description:					时间处理类
//
//		History:						Author									Date													Description
//											王科威									2018-05-02									创建
//

#ifndef TIME_H
#define TIME_H

#include "predef.h"

NS_BEGIN(time, 1)

class Time
{
public:
	Time(void);
	~Time(void);

	long long expiredMilliseconds(const std::string startTime = "1970-01-01 00:00:00");
};//class Time

NS_END

#endif//TIME_H
