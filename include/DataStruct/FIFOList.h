//
//		Copyright :					@2018, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2019-07-19
//		Description :				FIFO���ݽṹ����
//
//		History:						Author										Date													Description
//											������										2019-08-26									����
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

