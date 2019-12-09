//
//		Copyright :				@2017, HHJT, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2017-08-16
//		Description:				D3D设备
//
//		History:						Author									Date												Description
//											王科威									2017-08-16										创建
//

#ifndef D3D_DEVICE_H
#define D3D_DEVICE_H

#include "d3d9.h"
#include "d3dx9shader.h"

namespace framework
{
	namespace multimedia
	{
		class D3DDevice
		{
		public:
			D3DDevice(void);
			~D3DDevice(void);

			IDirect3DDevice9* create(IDirect3D9* d3d9 = NULL, const HWND hwnd = NULL, const int width = 0, const int height = 0);
			void destroy(IDirect3DDevice9*& device);
			DWORD* getFunctionCode(IDirect3DDevice9* device = NULL);
			int setPixelShader(IDirect3DDevice9* device = NULL, IDirect3DPixelShader9* shader = NULL);
			int setTexture(IDirect3DDevice9* device = NULL, ID3DXConstantTable* constant = NULL, IDirect3DTexture9** textures = NULL);
			int present(IDirect3DDevice9* device = NULL, IDirect3DVertexBuffer9* vertexBuffer = NULL);
		};//class D3DDevice
	}//namespace multimedia
}//namespace framework

#endif//D3D_DEVICE_H
