//
//		Copyright :				@2017, HHJT, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2017-08-16
//		Description:				D3D
//
//		History:						Author									Date												Description
//											王科威									2017-08-16										创建
//

#ifndef DIRECT_3D_H
#define DIRECT_3D_H

#include <string>
#include "d3d9.h"
#include "d3dx9core.h"
#include "predef.h"

NS_BEGIN(d3d, 1)

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

NS_END

#endif//DIRECT_3D_H
