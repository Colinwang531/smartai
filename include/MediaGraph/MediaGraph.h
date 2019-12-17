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

#include "boost/function.hpp"
#include "boost/noncopyable.hpp"
#include "boost/shared_ptr.hpp"
#include "DataStruct/UnorderedMap.h"

namespace framework
{
	namespace multimedia
	{
		class MediaFilter;
		using MediaFilterPtr = boost::shared_ptr<MediaFilter>;
		using MediaFilterGroup = UnorderedMap<const std::string, MediaFilterPtr>;
		typedef boost::function<void(const int, const unsigned char, const unsigned char*, const int, void*)> MediaFrameCallback;

		class MediaGraph : private boost::noncopyable
		{
		public:
			MediaGraph(void);
			virtual ~MediaGraph(void);

		public:
			int openStream(
				const std::string url, void* hwnd = NULL, MediaFrameCallback callback = NULL, void* userData = NULL);
			int closeStream(void);
			MediaFilterPtr queryMediaFilterByID(const std::string filterID);

		protected:
 			virtual int createAVCaptureFilter(void);
// 			virtual int createNewControllerFilter(const std::string streamURL);
// 			virtual int createNewDecoderFilter(const std::string streamURL);
// 			virtual int createNewConverterFilter(void);
// 			virtual int createNewRendererFilter(void* hwnd = NULL);
// 			virtual int createNewCallbackFilter(void);
// 			virtual int buildMediaGraph(void);

		protected:
			MediaFilterGroup mediaFilterGroup;
// 			MediaFrameCallback mediaFrameCallback;
// 			void* videoDisplayHwnd;
// 			void* userData;
		};//class MediaGraph
	}//namespace multimedia
}//namespace framework

#endif//MEDIA_GRAPH_H
