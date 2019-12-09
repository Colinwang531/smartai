#include "error.h"
#include "MediaModel/Renderer/DXS/AudioDxSoundPlayer.h"

NS_BEGIN(model, 1)

AudioDxSoundPlayer::AudioDxSoundPlayer() : MediaRenderer()
{}

AudioDxSoundPlayer::~AudioDxSoundPlayer()
{}

int AudioDxSoundPlayer::inputMediaData(MediaDataPtr mediaData)
{
	return ERR_NOT_SUPPORT;
}

// int AudioDxSoundPlayer::createNewMediaRenderer(void* /* = NULL */)
// {
// 	int status{ ERR_NOT_SUPPORT };
// 
// 	return status;
// }

NS_END
