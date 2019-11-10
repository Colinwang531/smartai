#include "boost/make_shared.hpp"
#include "error.h"
#include "MediaRenderer/D3D/VideoD3DRenderer.h"
using VideoD3DRenderer = NS(renderer, 1)::VideoD3DRenderer;
#include "MediaFilter/Renderer/AVRendererFilter.h"

NS_BEGIN(filter, 1)

AVRendererFilter::AVRendererFilter() : MediaFilter()
{}

AVRendererFilter::~AVRendererFilter()
{}

int AVRendererFilter::createNewMediaRenderer(void* hwnd /* = NULL */)
{
	int status{ ERR_OK };

	if (hwnd)
	{
		MediaRendererPtr videoRendererPtr{ boost::make_shared<VideoD3DRenderer>() };
		if (videoRendererPtr)
		{
			videoRendererPtr->createNewMediaRenderer(hwnd);
			mediaRendererPtr.swap(videoRendererPtr);
		}
	}
	else
	{
	}

	return ERR_OK == status ? createNewInputAndOutputPin(hwnd) : status;
}

int AVRendererFilter::inputMediaData(MediaDataPtr mediaData)
{
	int status{ mediaRendererPtr ? ERR_OK : ERR_BAD_OPERATE };

// 	if (ERR_OK == status)
// 	{
// 		// if current media data is the first one, create demuxer and output pins instances once.
// 		status = createNewMediaDemuxer(mediaData);
// 
// 		if (ERR_OK == status)
// 		{
// 			status = createNewOutputPin();
// 		}
// 	}

	return status;
}

int AVRendererFilter::createNewInputAndOutputPin(void* hwnd /* = NULL */)
{
	if (hwnd)
	{
		createNewInputPin(NS(pin, 1)::VideoStreamInputPinID);
		createNewOutputPin(NS(pin, 1)::VideoStreamOutputPinID);
	} 
	else
	{
		createNewInputPin(NS(pin, 1)::AudioStreamInputPinID);
		createNewOutputPin(NS(pin, 1)::AudioStreamOutputPinID);
	}

	return ERR_OK;
}

NS_END
