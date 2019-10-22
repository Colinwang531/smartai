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

#include "MediaFilter/MediaFilter.h"
using MediaFilterPtr = boost::shared_ptr<NS(filter, 1)::MediaFilter>;
using MediaFilterRef = boost::weak_ptr<NS(filter, 1)::MediaFilter>;
#include "DataStruct/UnorderedMap.h"
using MediaFilterGroup = UnorderedMap<const std::string, MediaFilterPtr>;
#include "MediaData/MediaData.h"
using MediaDataPtr = boost::shared_ptr<NS(media, 1)::MediaData>;

NS_BEGIN(graph, 1)

class MediaGraph
{
public:
	MediaGraph(void);
	virtual ~MediaGraph(void);

public:
	virtual MediaFilterRef queryFilterByID(const std::string filterID);
	//Add a new filter that was created by user.
	virtual int addFilter(const std::string filterID, MediaFilterPtr filterPtr);
	virtual int removeFilter(const std::string filterID);

protected:
	MediaFilterGroup mediaFilterGroup;
};//class MediaGraph

NS_END

#endif//MEDIA_GRAPH_H
