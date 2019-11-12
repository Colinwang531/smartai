// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Unordered map template.
//

#ifndef UNORDERED_MAP_H
#define UNORDERED_MAP_H

#include <vector>
#include "boost/unordered_map.hpp"
#include "Mutex/RWLock.h"

template <class Key, class Value>
class UnorderedMap
{
public:
	UnorderedMap(void) {}
	virtual ~UnorderedMap(void)
	{
		clear();
	}

public:
	void insert(Key k, Value v)
	{
		WriteLock wl{ mtx };
		umap.insert(std::make_pair(k, v));
	}

	void remove(Key k)
	{
		WriteLock wl{ mtx };
		typename boost::unordered_map<Key, Value>::const_iterator it{ umap.find(k) };

		if (umap.end() != it)
		{
			umap.erase(it);
		}
	}

	void clear(void)
	{
		WriteLock wl{ mtx };
		umap.clear();
	}
	
	Value at(Key k)
	{
		ReadLock rl{ mtx };
		Value ret{};
		typename boost::unordered_map<Key, Value>::const_iterator it{ umap.find(k) };

		if (umap.end() != it)
		{
			ret = it->second;
		}

		return ret;
	}

	unsigned long long size(void)
	{
		ReadLock rl{ mtx };
		return umap.size();
	}

	void values(std::vector<Value>& data)
	{
		WriteLock wl{ mtx };
		for (typename boost::unordered_map<Key, Value>::const_iterator it = umap.cbegin(); it != umap.cend(); ++it)
		{
			data.push_back(it->second);
		}
	}

private:
	boost::unordered_map<Key, Value> umap;
	SharedMutex mtx;
};//class UnorderedMap

#endif//UNORDERED_MAP_H

