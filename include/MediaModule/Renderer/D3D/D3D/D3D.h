// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Direct3D.
//

#ifndef DIRECT_3D_H
#define DIRECT_3D_H

#include <string>
#include "d3d9.h"
#include "d3dx9core.h"

namespace framework
{
	namespace multimedia
	{
		class D3D
		{
		public:
			D3D(void);
			~D3D(void);

			int init(const HWND hwnd = NULL, const int width = 0, const int height = 0);
			void uninit(void);
			int drawImage(
				const char* data = nullptr, const int width = 0, const int height = 0);
			int drawText(const std::string& text, const RECT& rect);

		private:
			IDirect3D9* createD3D9(const int version = D3D_SDK_VERSION);
			void destoryD3D9(IDirect3D9*& d3d9);

		private:
			IDirect3D9* d3d9;
			IDirect3DDevice9* d3d9Device;
			IDirect3DPixelShader9* d3d9PixelShader;
			IDirect3DVertexBuffer9* d3d9VertexBuffer;
			IDirect3DTexture9* d3d9Texture[3];
			ID3DXFont* d3dxFont;
		};//class D3D
	}//namespace multimedia
}//namespace framework

#endif//DIRECT_3D_H
