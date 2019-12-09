//
//		Copyright :				@2017, HHJT, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2017-08-16
//		Description:				D3D像素着色器
//
//		History:						Author									Date												Description
//											王科威									2017-08-16										创建
//

#ifndef D3D_PIXEL_SHADER_H
#define D3D_PIXEL_SHADER_H

#include "d3d9.h"
#include "d3dx9shader.h"

namespace framework
{
	namespace multimedia
	{
		class D3DPixelShader
		{
		public:
			D3DPixelShader(void);
			~D3DPixelShader(void);

			IDirect3DPixelShader9* create(IDirect3DDevice9* device = NULL);
			void destroy(IDirect3DPixelShader9*& shader);
			ID3DXConstantTable* getShaderConstantTable(IDirect3DDevice9* device = NULL);
		};//class D3DPixelShader
	}//namespace multimedia
}//namespace framework

#endif//D3D_PIXEL_SHADER_H

