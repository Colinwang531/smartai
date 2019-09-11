//
//		Copyright :					@2018, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2019-07-19
//		Description :				FIFO数据结构基类
//
//		History:						Author										Date													Description
//											王科威										2019-08-26									创建
//

#ifndef FIFO_LIST_H
#define FIFO_LIST_H

#include <list>
#include <mutex>
#include "predef.h"

NS_BEGIN(datastruct, 1)

class FIFOList
{
public:
	FIFOList(const int capacity = 25);
	virtual ~FIFOList(void);

public:
	int pushBack(void* element = NULL);
	void popFront(void);
	void* getFront(void);
	bool isFull(void);

private:
	const int elementCapacity;
	std::list<void*> elements;
	std::mutex rwLock;
};//class FIFOList

NS_END

#endif//FIFO_LIST_H

