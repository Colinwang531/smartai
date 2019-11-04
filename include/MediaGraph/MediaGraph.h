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
#include "MediaData/MediaData.h"
using MediaDataPtr = boost::shared_ptr<NS(media, 1)::MediaData>;

NS_BEGIN(graph, 1)

class MediaGraph : private boost::noncopyable
{
public:
	MediaGraph(void);
	virtual ~MediaGraph(void);

public:
	virtual MediaFilterRef queryMediaFilterByID(const std::string filterID);
	//Modify filter that created by user in the graph.
	virtual int addMediaFilter(const std::string filterID, MediaFilterPtr mediaFilter);
	virtual int removeMediaFilter(const std::string filterID);
	virtual int inputMediaData(MediaDataPtr mediaData) = 0;

protected:
	virtual int buildMediaGraphByMediaStreams(void) = 0;

protected:
	MediaFilterGroup mediaFilterGroup;
};//class MediaGraph

NS_END

#endif//MEDIA_GRAPH_H
