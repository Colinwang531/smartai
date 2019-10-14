//
//		Copyright :					@2019, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2019-10-13
//		Description :				HASH映射表类
//
//		History:						Author										Date													Description
//											王科威										2019-10-13									创建
//

#ifndef UNORDERED_MAP_H
#define UNORDERED_MAP_H

#include "boost/thread/mutex.hpp"
#include "boost/unordered_map.hpp"
#include "predef.h"

NS_BEGIN(datastruct, 1)

template <class Key, class Value>
class UnorderedMap
{
public:
	UnorderedMap(void) : capacity{ 0xFFFF }
	{}
	~UnorderedMap(void)
	{}

public:
	void setCapacity(const unsigned long long number = 0xFFFF) 
	{
		capacity = number;
	}

	bool insert(Key k, Value v)
	{
		bool status{ false };

		mtx.lock();
		if (capacity > queue.size())
		{
			queue.insert(k, v);
			status = true;
		}
		mtx.unlock();

		return status;
	}

	bool remove(Key k)
	{
		bool status{ false };

		mtx.lock();
		boost::unordered_map<Key, Value>::iterator it{ queue.find(k) };
		if (queue.end() != it)
		{
			queue.erase(it);
			status = true;
		}
		mtx.unlock();

		return status;
	}
	
	Value find(Key k)
	{
		Value v;

		mtx.lock();
		boost::unordered_map<Key, Value>::iterator it{ queue.find(k) };
		if (queue.end() != it)
		{
			v = it->second;
		}
		mtx.unlock();

		return status;
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
	boost::unordered_map<Key, Value> queue;
	boost::mutex mtx;
};//class FIFOQueue

NS_END

#endif//UNORDERED_MAP_H

