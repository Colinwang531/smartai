// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Media play controller base class.
//

#ifndef MEDIA_PLAY_CONTROLLER_H
#define MEDIA_PLAY_CONTROLLER_H

#include "predef.h"

NS_BEGIN(graph, 1)

class EnableMediaPlayControl
{
protected:
	EnableMediaPlayControl(void) {}
	virtual ~EnableMediaPlayControl(void) {}

protected:
	virtual int play(void) = 0;
	virtual int stop(void) = 0;
	virtual int pause(void) = 0;
	//speed range : 1x / 2x /4x / 8x / 16x
	virtual int fast(const unsigned short speed = 1) = 0;
	virtual int slow(const unsigned short speed = 1) = 0;
};//class NetworkFileMediaGraph

NS_END

#endif//MEDIA_PLAY_CONTROLLER_H
