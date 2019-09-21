//
//		Copyright :					@2018, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2019-07-19
//		Description :				FIFO队列类
//
//		History:						Author										Date													Description
//											王科威										2019-08-26									创建
//

#ifndef FIFO_QUEUE_H
#define FIFO_QUEUE_H

#include "boost/thread/mutex.hpp"
#include "predef.h"

NS_BEGIN(datastruct, 1)

template <class T>
class FIFOQueue
{
public:
	FIFOQueue(void) : capacity{ 0xFFFF }
	{}
	~FIFOQueue(void)
	{}

public:
	void setCapacity(const unsigned long long number = 0xFFFF) 
	{
		capacity = number;
	}

	bool insert(const T e)
	{
		bool status{ false };

		mtx.lock();
		if (capacity > queue.size())
		{
			queue.push_back(e);
			status = true;
		}
		mtx.unlock();

		return status;
	}

	T remove(void)
	{
		T e{};

		mtx.lock();
		if (0 < queue.size())
		{
			e = queue[0];
			queue.erase(queue.begin());
		}
		mtx.unlock();

		return e;
	}
	
	T front(void)
	{
		T e;

		mtx.lock();
		if (0 < queue.size())
		{
			e = queue[0];
		}
		mtx.unlock();

		return e;
	}

	unsigned long long size(void)
	{
		unsigned long long elementNumber{ 0 };

		mtx.lock();
		elementNumber = queue.size();
		mtx.unlock();

		return elementNumber;
	}

	inline unsigned long long total(void) const
	{
		return capacity;
	}

	void swap(std::vector<T>& out)
	{
		mtx.lock();
		out.swap(queue);
		mtx.unlock();
	}

private:
	unsigned long long capacity;
	std::vector<T> queue;
	boost::mutex mtx;
};//class FIFOQueue

NS_END

#endif//FIFO_QUEUE_H

