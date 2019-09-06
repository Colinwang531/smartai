//
//		Copyright :					@2017, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2019-07-22
//		Description:					文件读操作抽象基类定义
//
//		History:						Author									Date											Description
//											王科威									2019-07-22							创建
//

#ifndef READING_H
#define READING_H

#include <string>
#include "predef.h"

NS_BEGIN(file, 1)

class Reading
{
public:
	Reading(void) {}
	virtual ~Reading(void){}

public:
	virtual Int32 readingIntValueByKey(const std::string key) = 0;
	virtual std::string readingStringValueByKey(const std::string key) = 0;
};//class Reading

NS_END

#endif//READING_H
