//
//		Copyright :				@2017, HHJT, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2017-08-16
//		Description:				D3D顶点缓冲
//
//		History:						Author									Date												Description
//											王科威									2017-08-16										创建
//

#ifndef D3D_VERTEX_BUFFER_H
#define D3D_VERTEX_BUFFER_H

#include "d3d9.h"
#include "d3dx9core.h"

namespace framework
{
	namespace multimedia
	{
		typedef struct tagD3DVertex_t
		{
			D3DXVECTOR4 pos;
			DWORD color;
			D3DXVECTOR2 tex1;
			D3DXVECTOR2 tex2;
			D3DXVECTOR2 tex3;
		}D3DVertex;

		class D3DVertexBuffer
		{
		public:
			D3DVertexBuffer(void);
			~D3DVertexBuffer(void);

			IDirect3DVertexBuffer9* create(IDirect3DDevice9* device = NULL, const int width = 0, const int height = 0);
			void destroy(IDirect3DVertexBuffer9*& vertexBuffer);
		};//class D3DVertexBuffer
	}//namespace multimedia
}//namespace framework

#endif//D3D_VERTEX_BUFFER_H

