// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Media play controller interface class.
//

#ifndef MEDIA_PLAY_CONTROLLER_H
#define MEDIA_PLAY_CONTROLLER_H

#include "predef.h"

NS_BEGIN(graph, 1)

typedef enum class tagMediaPlayStatus_t
{
	MEDIA_PLAY_STATUS_STOP = 0,
	MEDIA_PLAY_STATUS_PLAY,
	MEDIA_PLAY_STATUS_PAUSE,
}MediaPlayStatus;

class EnableMediaPlayControl
{
protected:
	EnableMediaPlayControl(void) : 
		playStatus{ MediaPlayStatus::MEDIA_PLAY_STATUS_STOP }, playSpeed{ 0 }
	{}
	virtual ~EnableMediaPlayControl(void) {}

protected:
	virtual int startPlay(void) = 0;
	virtual int stopPlay(void) = 0;
	virtual int pausePlay(void) = 0;
	virtual int fastPlay(const short speed = 1) = 0;
	virtual int slowPlay(const short speed = -1) = 0;
	inline void getStatus(MediaPlayStatus& status, short& speed) const
	{
		status = playStatus;
		speed = playSpeed;
	}

protected:
	// It is 0 while start or stop playing media, 
	// fast speed range is 1x / 2x / 4x / 8x / 16x,
	// slow speed range is -1x / -2x / -4x / -8x / -16x.
	short playSpeed;
	MediaPlayStatus playStatus;
};//class NetworkFileMediaGraph

NS_END

#endif//MEDIA_PLAY_CONTROLLER_H
