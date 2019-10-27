// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Array template.
//

#ifndef VECTOR_H
#define VECTOR_H

#include <vector>
#include "Mutex/RWLock.h"

template <class T>
class Vector
{
public:
	Vector(void) {}
	virtual ~Vector(void) 
	{
		clear();
	}

public:
	void insert(const T e)
	{
		WriteLock wl{ mtx };
		queue.push_back(e);
	}

	void remove(void)
	{
		WriteLock wl{ mtx };
		if (0 < queue.size())
		{
			queue.erase(queue.begin());
		}
	}

	void clear(void)
	{
		WriteLock wl{ mtx };
		queue.clear();
	}
	
	const T& at(const unsigned long long index = 0)
	{
		T e;

		ReadLock rl{ mtx };
		if (index < queue.size())
		{
			e = queue[index];
		}

		return e;
	}

	unsigned long long size(void)
	{
		ReadLock rl{ mtx };
		return queue.size();
	}

private:
	std::vector<T> queue;
	SharedMutex mtx;
};//class FIFOQueue

#endif//VECTOR_H

