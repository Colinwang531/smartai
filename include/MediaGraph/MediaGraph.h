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
#include "DataStruct/UnorderedMap.h"

namespace framework
{
	namespace multimedia
	{
		class MediaFilter;
		using MediaFilterGroup = UnorderedMap<const std::string, MediaFilter*>;

		class MediaGraph : private boost::noncopyable
		{
		public:
			MediaGraph(void);
			virtual ~MediaGraph(void);

		public:
			virtual int createNewGraph(void) = 0;
			virtual int destroyGraph(void) = 0;
			virtual MediaFilter* queryMediaFilterByID(const std::string& filterID);

		protected:
			virtual int postCreateNewGraph(void) = 0;
			int addMediaFilter(const std::string& filterID, MediaFilter* filter = NULL);
			int removeMediaFilter(const std::string& filterID);

		protected:
			MediaFilterGroup mediaFilterGroup;
		};//class MediaGraph
	}//namespace multimedia
}//namespace framework

#endif//MEDIA_GRAPH_H
