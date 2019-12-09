// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Abstract base class of filter module.
//

#ifndef MEDIA_MODULE_H
#define MEDIA_MODULE_H

#include "boost/function.hpp"
#include "boost/shared_ptr.hpp"

namespace framework
{
	namespace multimedia
	{
		class MediaData;
		using MediaDataPtr = boost::shared_ptr<MediaData>;
		typedef boost::function<int(MediaDataPtr)> PostInputMediaDataCallback;

		class MediaModule
		{
		public:
			MediaModule(void);
			virtual ~MediaModule(void);

		public:
			void setPostInputMediaDataCallback(PostInputMediaDataCallback callback = NULL);
			virtual int inputMediaData(MediaDataPtr mediaData) = 0;

		protected:
			PostInputMediaDataCallback postInputMediaDataCallback;
		};//class MediaModule
	}//namespace multimedia
}//namespace framework

#endif//MEDIA_MODULE_H
