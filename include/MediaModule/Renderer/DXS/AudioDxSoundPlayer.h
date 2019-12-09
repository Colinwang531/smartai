// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Audio playing using DirectX sound library.
//

#ifndef AUDIO_DX_SOUND_PLAYER_H
#define AUDIO_DX_SOUND_PLAYER_H

#include "MediaModel/Renderer/MediaRenderer.h"

NS_BEGIN(model, 1)

class AudioDxSoundPlayer : public MediaRenderer
{
public:
	AudioDxSoundPlayer(void);
	virtual ~AudioDxSoundPlayer(void);

protected:
	int inputMediaData(MediaDataPtr mediaData) override;
//	int createNewMediaRenderer(void* = NULL) override;
};//class VideoD3DRenderer

NS_END

#endif//AUDIO_DX_SOUND_PLAYER_H
