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
#include "boost/shared_ptr.hpp"
#include "boost/weak_ptr.hpp"
#include "DataStruct/UnorderedMap.h"

namespace framework
{
	namespace multimedia
	{
		class MediaFilter;
		using MediaFilterPtr = boost::shared_ptr<MediaFilter>;
		using MediaFilterRef = boost::weak_ptr<MediaFilter>;
		using MediaFilterGroup = UnorderedMap<const std::string, MediaFilterPtr>;

		class MediaGraph : private boost::noncopyable
		{
		public:
			MediaGraph(void);
			virtual ~MediaGraph(void);

		public:
			virtual int createNewGraph(void) = 0;
			virtual int destroyGraph(void);
			MediaFilterRef queryMediaFilterByID(const std::string& filterID);

		protected:
			int addMediaFilter(const std::string& filterID, MediaFilterPtr filter);
			int removeMediaFilter(const std::string& filterID);

		protected:
			MediaFilterGroup mediaFilterGroup;
		};//class MediaGraph
	}//namespace multimedia
}//namespace framework

#endif//MEDIA_GRAPH_H
