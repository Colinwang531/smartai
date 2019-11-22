// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Base class of live stream accessor.
//

#ifndef MEDIA_ACCESSOR_H
#define MEDIA_ACCESSOR_H

#include "MediaModel/MediaModel.h"

NS_BEGIN(model, 1)

class MediaAccessor : public MediaModel
{
public:
	MediaAccessor(void);
	virtual ~MediaAccessor(void);

public:
	virtual int openStream(
		const std::string name, const std::string password, 
		const std::string address, const int port = 8000,
		const int channel = 0, void* hwnd = NULL);
	virtual int closeStream(void);
};//class MediaAccessor

NS_END

#endif//MEDIA_ACCESSOR_H
