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
#include "DataStruct/UnorderedMap.h"
using MediaFilterGroup = UnorderedMap<const std::string, MediaFilterPtr>;

NS_BEGIN(graph, 1)

class MediaGraph : private boost::noncopyable
{
public:
	MediaGraph(void);
	virtual ~MediaGraph(void);

public:
	// @streamUrl : URL for opening media.
	// @hwnd : Window handle for displaying video.
	virtual int openMediaGraph(const std::string streamUrl, void* hwnd = NULL) = 0;
	virtual int closeMediaGraph(void) = 0;
	virtual MediaFilterPtr queryMediaFilterByID(const std::string filterID);

protected:
	//Modify filter that created by user in the graph.
	virtual int addMediaFilter(const std::string filterID, MediaFilterPtr mediaFilter);
	virtual int removeMediaFilter(const std::string filterID);

private:
	MediaFilterGroup mediaFilterGroup;
};//class MediaGraph

NS_END

#endif//MEDIA_GRAPH_H
