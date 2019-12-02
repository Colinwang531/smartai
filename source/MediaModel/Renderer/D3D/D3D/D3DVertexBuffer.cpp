#include "MediaModel/Renderer/D3D/D3D/D3DVertexBuffer.h"

namespace framework
{
	namespace multimedia
	{
		D3DVertexBuffer::D3DVertexBuffer()
		{}

		D3DVertexBuffer::~D3DVertexBuffer()
		{}

		IDirect3DVertexBuffer9* D3DVertexBuffer::create(IDirect3DDevice9* device /* = NULL */, const int width /* = 0 */, const int height /* = 0 */)
		{
			IDirect3DVertexBuffer9* tempOfD3DVertexBuffer9{ NULL };

			if (device && 0 < width && 0 < height)
			{
				if (SUCCEEDED(device->CreateVertexBuffer(
					3 * sizeof(D3DVertex), D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX3, D3DPOOL_DEFAULT, &tempOfD3DVertexBuffer9, NULL)))
				{
					D3DVertex* tempOfD3DVertex{ NULL };

					if (SUCCEEDED(tempOfD3DVertexBuffer9->Lock(0, 0, (void**)&tempOfD3DVertex, 0)))
					{
						long x = 0;
						long y = 0;
						long dx = width;
						long dy = height;
						tempOfD3DVertex[0].color = tempOfD3DVertex[1].color = tempOfD3DVertex[2].color = tempOfD3DVertex[3].color = D3DCOLOR_RGBA(255, 255, 255, 255);

						tempOfD3DVertex[0].pos = D3DXVECTOR4(x - 0.5f, y - 0.5f, 0.0f, 1.0f);
						tempOfD3DVertex[1].pos = D3DXVECTOR4(dx - 0.5f, y - 0.5f, 0.0f, 1.0f);
						tempOfD3DVertex[2].pos = D3DXVECTOR4(dx - 0.5f, dy - 0.5f, 0.0f, 1.0f);
						tempOfD3DVertex[3].pos = D3DXVECTOR4(x - 0.5f, dy - 0.5f, 0.0f, 1.0f);

						//				if (m_Flip == ID3DVRInterface::Not_Upper_Down_Flip)
						//				{
						tempOfD3DVertex[0].tex1 = tempOfD3DVertex[0].tex2 = tempOfD3DVertex[0].tex3 = D3DXVECTOR2(0.0f, 0.0f);
						tempOfD3DVertex[1].tex1 = tempOfD3DVertex[1].tex2 = tempOfD3DVertex[1].tex3 = D3DXVECTOR2(1.0f, 0.0f);
						tempOfD3DVertex[2].tex1 = tempOfD3DVertex[2].tex2 = tempOfD3DVertex[2].tex3 = D3DXVECTOR2(1.0f, 1.0f);
						tempOfD3DVertex[3].tex1 = tempOfD3DVertex[3].tex2 = tempOfD3DVertex[3].tex3 = D3DXVECTOR2(0.0f, 1.0f);
						//				}
						// 				else
						// 				{
						// 					direct3DVertex[0].tex1 = direct3DVertex[0].tex2 = direct3DVertex[0].tex3 = D3DXVECTOR2(0.0f, 1.0f);
						// 					direct3DVertex[1].tex1 = direct3DVertex[1].tex2 = direct3DVertex[1].tex3 = D3DXVECTOR2(1.0f, 1.0f);
						// 					direct3DVertex[2].tex1 = direct3DVertex[2].tex2 = direct3DVertex[2].tex3 = D3DXVECTOR2(1.0f, 0.0f);
						// 					direct3DVertex[3].tex1 = direct3DVertex[3].tex2 = direct3DVertex[3].tex3 = D3DXVECTOR2(0.0f, 0.0f);
						// 				}

						tempOfD3DVertexBuffer9->Unlock();
					}
				}
			}

			return tempOfD3DVertexBuffer9;
		}

		void D3DVertexBuffer::destroy(IDirect3DVertexBuffer9*& vertexBuffer)
		{
			if (vertexBuffer)
			{
				vertexBuffer->Release();
				vertexBuffer = NULL;
			}
		}
	}//namespace multimedia
}//namespace framework
