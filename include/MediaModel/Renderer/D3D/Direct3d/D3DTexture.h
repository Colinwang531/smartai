//
//		Copyright :				@2017, HHJT, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2017-08-16
//		Description:				D3D图片
//
//		History:						Author									Date												Description
//											王科威									2017-08-16										创建
//

#ifndef D3D_TEXTURE_H
#define D3D_TEXTURE_H

#include "d3d9.h"
#include "predef.h"

NS_BEGIN(d3d, 1)

class D3DTexture
{
public:
	D3DTexture(void);
	~D3DTexture(void);

	IDirect3DTexture9* create(IDirect3DDevice9* device = NULL, const int width = 0, const int height = 0);
	void destroy(IDirect3DTexture9* texture);
	int drawImage(IDirect3DTexture9* texture = NULL, const char* data = NULL, const int width = 0, const int height = 0);
};//class D3DTexture

NS_END

#endif//D3D_TEXTURE_H

