#include "boost/make_shared.hpp"
#include "error.h"
#include "MediaModel/Renderer/D3D/VideoD3DRenderer.h"

NS_BEGIN(model, 1)

VideoD3DRenderer::VideoD3DRenderer(const HWND hwnd /* = NULL */)
	: MediaRenderer(), videoDisplayWnd{ hwnd }
{}

VideoD3DRenderer::~VideoD3DRenderer()
{}

int VideoD3DRenderer::inputMediaData(MediaDataPtr mediaData)
{
	int status{ mediaData ? ERR_OK : ERR_INVALID_PARAM };

	if (ERR_OK == status)
	{
		if (!direct3dPtr)
		{
			status = createNewMediaRenderer(mediaData);
		}

		if (ERR_OK == status && direct3dPtr)
		{
			status = direct3dPtr->drawImage((const char*)mediaData->getData(), mediaData->getWidth(), mediaData->getHeight());

			RECT rect;
			rect.top = 10;
			rect.left = 10;
			rect.right = 1000;
			rect.bottom = 100;
			direct3dPtr->drawText("Display for testing.", rect);
		}
	}

	return status;
}

int VideoD3DRenderer::createNewMediaRenderer(MediaDataPtr mediaData)
{
	int status{ videoDisplayWnd ? ERR_OK : ERR_INVALID_PARAM };

	if (ERR_OK == status)
	{
		Direct3DPtr d3dPtr{ boost::make_shared<NS(d3d, 1)::D3D>() };
		if (d3dPtr)
		{
			status = d3dPtr->init(reinterpret_cast<HWND>(videoDisplayWnd), mediaData->getWidth(), mediaData->getHeight());
			if (ERR_OK == status)
			{
				direct3dPtr.swap(d3dPtr);
			}
		}
		else
		{
			status = ERR_BAD_ALLOC;
		}
	}

	return status;
}

NS_END
