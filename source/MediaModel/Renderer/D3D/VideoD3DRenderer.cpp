#include "boost/make_shared.hpp"
#include "error.h"
#include "MediaData/MediaData.h"
#include "MediaModel/Renderer/D3D/VideoD3DRenderer.h"

namespace framework
{
	namespace multimedia
	{
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
					int width{ 0 }, height{ 0 };
					mediaData->getImageParameter(width, height);
					status = direct3dPtr->drawImage((const char*)mediaData->getData(), width, height);

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
				Direct3DPtr d3dPtr{ boost::make_shared<framework::multimedia::D3D>() };
				if (d3dPtr)
				{
					int width{ 0 }, height{ 0 };
					mediaData->getImageParameter(width, height);
					status = d3dPtr->init(reinterpret_cast<HWND>(videoDisplayWnd), width, height);
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
	}//namespace multimedia
}//namespace framework
