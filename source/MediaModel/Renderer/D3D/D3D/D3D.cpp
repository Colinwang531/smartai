#include <algorithm>
#include "error.h"
#include "MediaModel/Renderer/D3D/D3D/D3DDevice.h"
#include "MediaModel/Renderer/D3D/D3D/D3DPixelShader.h"
#include "MediaModel/Renderer/D3D/D3D/D3DVertexBuffer.h"
#include "MediaModel/Renderer/D3D/D3D/D3DTexture.h"
#include "MediaModel/Renderer/D3D/D3D/D3DFont.h"
#include "MediaModel/Renderer/D3D/D3D/D3D.h"

namespace framework
{
	namespace multimedia
	{
		D3D::D3D() : d3d9{ NULL }, d3d9Device{ NULL }, d3d9PixelShader{ NULL }, d3d9VertexBuffer{ NULL }, d3dxFont{ NULL }
		{
			d3d9Texture[0] = d3d9Texture[1] = d3d9Texture[2] = NULL;
		}

		D3D::~D3D()
		{}

		int D3D::init(const HWND hwnd /* = NULL */, const int width /* = 0 */, const int height /* = 0 */)
		{
			int status{ ERR_BAD_ALLOC };
			IDirect3D9* tempOfD3d9{ createD3D9() };
			IDirect3DDevice9* tempOfD3d9Device{ D3DDevice().create(tempOfD3d9, hwnd, width, height) };
			IDirect3DPixelShader9* tempOfD3d9PixelShader{ D3DPixelShader().create(tempOfD3d9Device) };
			IDirect3DVertexBuffer9* tempOfD3d9VertexBuffer{ D3DVertexBuffer().create(tempOfD3d9Device, width, height) };
			IDirect3DTexture9* tempOfD3D9Texture[3]{
				D3DTexture().create(tempOfD3d9Device, width, height),
				D3DTexture().create(tempOfD3d9Device, width / 2, height / 2),
				D3DTexture().create(tempOfD3d9Device, width / 2, height / 2) };
			ID3DXFont* tempOfD3DXFont{ D3DFont().create(tempOfD3d9Device) };

			if (tempOfD3d9 && tempOfD3d9Device && tempOfD3d9PixelShader && tempOfD3d9VertexBuffer && tempOfD3DXFont &&
				tempOfD3D9Texture[0] && tempOfD3D9Texture[1] && tempOfD3D9Texture[2])
			{
				std::swap(d3d9Device, tempOfD3d9Device);
				std::swap(d3d9PixelShader, tempOfD3d9PixelShader);
				std::swap(d3d9VertexBuffer, tempOfD3d9VertexBuffer);
				std::swap(d3d9Texture, tempOfD3D9Texture);
				std::swap(d3dxFont, tempOfD3DXFont);

				ID3DXConstantTable* tempOfConstantTable{ D3DPixelShader().getShaderConstantTable(d3d9Device) };
				if (tempOfConstantTable)
				{
					status = D3DDevice().setPixelShader(d3d9Device, d3d9PixelShader);
					status = D3DDevice().setTexture(d3d9Device, tempOfConstantTable, d3d9Texture);
					tempOfConstantTable->Release();
				}

				status = ERR_OK;
			}
			else
			{
				for (int i = 0; i != 3; ++i)
				{
					D3DTexture().destroy(tempOfD3D9Texture[i]);
				}
				D3DVertexBuffer().destroy(tempOfD3d9VertexBuffer);
				D3DPixelShader().destroy(tempOfD3d9PixelShader);
				D3DFont().destroy(tempOfD3DXFont);
				D3DDevice().destroy(tempOfD3d9Device);
				tempOfD3d9->Release();
			}

			return status;
		}

		void D3D::uninit(void)
		{
			for (int i = 0; i != 3; ++i)
			{
				D3DTexture().destroy(d3d9Texture[i]);
			}
			D3DVertexBuffer().destroy(d3d9VertexBuffer);
			D3DPixelShader().destroy(d3d9PixelShader);
			D3DFont().destroy(d3dxFont);
			D3DDevice().destroy(d3d9Device);
			// 	d3d9->Release();
			// 	d3d9 = NULL;
		}

		int D3D::drawImage(const char* data /* = nullptr */, const int width /* = 0 */, const int height /* = 0 */)
		{
			int tempOfPositionForU{ 0 }, tempOfPositionForV{ 0 };

			//	if (FrameType::FrameIYUV == mediaData.frame)
			{
				tempOfPositionForU = width * height;
				tempOfPositionForV = tempOfPositionForU * 5 / 4;
			}
			// 	else if (FrameType::FrameYV12 == mediaData.frame)
			// 	{
			// 		tempOfPositionForU = tempOfPositionForU * 5 / 4;
			// 		tempOfPositionForV = width * height;
			// 	}

			D3DTexture().drawImage(d3d9Texture[0], data, width, height);
			D3DTexture().drawImage(d3d9Texture[1], data + tempOfPositionForU, width / 2, height / 2);
			D3DTexture().drawImage(d3d9Texture[2], data + tempOfPositionForV, width / 2, height / 2);

			return D3DDevice().present(d3d9Device, d3d9VertexBuffer);
		}

		int D3D::drawText(const std::string& text, const RECT& rect)
		{
			return D3DFont().drawText(text, rect, d3dxFont);
		}

		IDirect3D9* D3D::createD3D9(const int version /* = D3D_SDK_VERSION */)
		{
			return Direct3DCreate9(version);
		}

		void D3D::destoryD3D9(IDirect3D9*& d3d9)
		{
			if (d3d9)
			{
				d3d9->Release();
				d3d9 = NULL;
			}
		}
	}//namespace multimedia
}//namespace framework
