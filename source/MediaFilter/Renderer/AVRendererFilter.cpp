#include "boost/make_shared.hpp"
#include "error.h"
#include "MediaModel/Renderer/D3D/VideoD3DRenderer.h"
using VideoD3DRenderer = NS(model, 1)::VideoD3DRenderer;
#include "MediaModel/Renderer/DXS/AudioDxSoundPlayer.h"
using AudioDxSoundPlayer = NS(model, 1)::AudioDxSoundPlayer;
#include "MediaFilter/Renderer/AVRendererFilter.h"

NS_BEGIN(filter, 1)

AVRendererFilter::AVRendererFilter(const HWND hwnd /* = NULL */)
	: MediaFilter(), videoDisplayWnd{ hwnd }
{}

AVRendererFilter::~AVRendererFilter()
{}

int AVRendererFilter::createNewFilter()
{
	if (videoDisplayWnd)
	{
		MediaModelPtr dxVideoD3dRendererPtr{ boost::make_shared<VideoD3DRenderer>(videoDisplayWnd) };
		if (dxVideoD3dRendererPtr)
		{
			createNewInputPin(NS(pin, 1)::VideoStreamInputPinID);
			createNewOutputPin(NS(pin, 1)::VideoStreamOutputPinID);
			mediaModelPtr.swap(dxVideoD3dRendererPtr);
		}
	}
	else
	{
		MediaModelPtr dxSoundPlayerPtr{ boost::make_shared<AudioDxSoundPlayer>() };
		if (dxSoundPlayerPtr)
		{
			createNewInputPin(NS(pin, 1)::AudioStreamInputPinID);
			createNewOutputPin(NS(pin, 1)::AudioStreamOutputPinID);
			mediaModelPtr.swap(dxSoundPlayerPtr);
		}
	}

	return MediaFilter::createNewFilter();
}

int AVRendererFilter::destroyFilter()
{
	return ERR_OK;
}

int AVRendererFilter::inputMediaData(MediaDataPtr mediaData)
{
	return mediaModelPtr ? mediaModelPtr->inputMediaData(mediaData) : ERR_INVALID_PARAM;
}

NS_END
