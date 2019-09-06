//
//		Copyright :					@2017, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2019-07-22
//		Description:					文件写操作抽象基类定义
//
//		History:						Author									Date											Description
//											王科威									2019-07-22							创建
//

#ifndef WRITTING_H
#define WRITTING_H

#include <string>
#include "predef.h"

NS_BEGIN(file, 1)

class Writting
{
public:
	Writting(void) {}
	virtual ~Writting(void) {}

public:
	virtual Int32 writtingWithLevel(const Int32 level, const std::string format, ...) = 0;
};//class Writting

NS_END

#endif//WRITTING_H
