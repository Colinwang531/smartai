#include "boost/make_shared.hpp"
#include "error.h"
#include "MediaModel/Renderer/D3D/VideoD3DRenderer.h"
using VideoD3DRenderer = NS(model, 1)::VideoD3DRenderer;
#include "MediaModel/Renderer/DXS/AudioDxSoundPlayer.h"
using AudioDxSoundPlayer = NS(model, 1)::AudioDxSoundPlayer;
#include "MediaFilter/Renderer/AVRendererFilter.h"

NS_BEGIN(filter, 1)

AVRendererFilter::AVRendererFilter(const AVRendererType type /* = AVRendererType::AV_RENDERER_TYPE_NONE */)
	: MediaFilter(), rendererType{ type }
{}

AVRendererFilter::~AVRendererFilter()
{}

int AVRendererFilter::createNewFilter()
{
	int status{ ERR_BAD_ALLOC };

	if (AVRendererType::AV_RENDERER_TYPE_VIDEO == rendererType)
	{
		if (ERR_OK == createNewInputPin(NS(pin, 1)::VideoStreamInputPinID) &&
			ERR_OK == createNewOutputPin(NS(pin, 1)::VideoStreamOutputPinID))
		{
			MediaModelPtr dxVideoD3dRendererPtr{ boost::make_shared<VideoD3DRenderer>() };
			if (dxVideoD3dRendererPtr)
			{
				mediaModelPtr.swap(dxVideoD3dRendererPtr);
				status = ERR_OK;
			}
		}
	}
	else if (AVRendererType::AV_RENDERER_TYPE_AUDIO == rendererType)
	{
		if (ERR_OK == createNewInputPin(NS(pin, 1)::AudioStreamInputPinID) &&
			ERR_OK == createNewOutputPin(NS(pin, 1)::AudioStreamOutputPinID))
		{
			MediaModelPtr dxSoundPlayerPtr{ boost::make_shared<AudioDxSoundPlayer>() };
			if (dxSoundPlayerPtr)
			{
				mediaModelPtr.swap(dxSoundPlayerPtr);
				status = ERR_OK;
			}
		}
	}

	return ERR_OK == status ? MediaFilter::createNewFilter() : status;
}

int AVRendererFilter::destroyFilter()
{
	return ERR_OK;
}

int AVRendererFilter::inputMediaData(MediaDataPtr mediaData)
{
	return mediaData && mediaModelPtr ? mediaModelPtr->inputMediaData(mediaData) : ERR_INVALID_PARAM;
}

NS_END
