// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Abstract base class of graph.
//

#ifndef MEDIA_GRAPH_H
#define MEDIA_GRAPH_H

#include "boost/noncopyable.hpp"
#include "MediaFilter/MediaFilter.h"
using MediaFilterPtr = boost::shared_ptr<NS(filter, 1)::MediaFilter>;
using MediaFilterRef = boost::weak_ptr<NS(filter, 1)::MediaFilter>;
#include "DataStruct/UnorderedMap.h"
using MediaFilterGroup = UnorderedMap<const std::string, MediaFilterPtr>;

NS_BEGIN(graph, 1)

class MediaGraph : private boost::noncopyable
{
public:
	MediaGraph(void);
	virtual ~MediaGraph(void);

public:
	virtual int createNewGraph(void* hwnd = NULL, void* callback = NULL) = 0;
	virtual int destroyGraph(void) = 0;
	virtual MediaFilterRef queryMediaFilterByID(const std::string filterID);

protected:
	int addMediaFilter(const std::string filterID, MediaFilterPtr mediaFilter);
	int removeMediaFilter(const std::string filterID);

protected:
	MediaFilterGroup mediaFilterGroup;
	void* displayHwnd;
	void* userCallback;
};//class MediaGraph

NS_END

#endif//MEDIA_GRAPH_H
