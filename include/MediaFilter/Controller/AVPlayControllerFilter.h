// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Controller of video and audio for playing stream.
//

#ifndef AV_PLAY_CONTROLLER_FILTER_H
#define AV_PLAY_CONTROLLER_FILTER_H

#include "MediaFilter/MediaFilter.h"

NS_BEGIN(filter, 1)

class AVPlayControllerFilter : public MediaFilter
{
public:
	AVPlayControllerFilter(void);
	virtual ~AVPlayControllerFilter(void);

public:
	int createNewMediaController(void);
	int inputMediaData(MediaDataPtr mediaData) override;
	int startPlay(void);
	int stopPlay(void);
	int pausePlay(void);
	int fastPlay(const short speed = 1);
	int slowPlay(const short speed = -1);

private:
	int checkSpeedValue(const short speed = 0);
};//class AVPlayControllerFilter

NS_END

#endif//AV_PLAY_CONTROLLER_FILTER_H
