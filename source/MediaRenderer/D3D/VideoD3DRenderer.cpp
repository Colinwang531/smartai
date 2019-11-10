#include "error.h"
#include "MediaRenderer/D3D/VideoD3DRenderer.h"

NS_BEGIN(renderer, 1)

VideoD3DRenderer::VideoD3DRenderer() : MediaRenderer()
{}

VideoD3DRenderer::~VideoD3DRenderer()
{}

int VideoD3DRenderer::createNewMediaRenderer(void* hwnd /* = NULL */)
{
	int status{ ERR_NOT_SUPPORT };

	return status;
}

NS_END
