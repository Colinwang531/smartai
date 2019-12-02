//
//		Copyright :				@2017, HHJT, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2017-08-16
//		Description:				D3D字体
//
//		History:						Author									Date												Description
//											王科威									2017-08-16										创建
//
#ifndef D3D_FONT_H
#define D3D_FONT_H

#include <string>
#include "d3d9.h"
#include "d3dx9core.h"

namespace framework
{
	namespace multimedia
	{
		class D3DFont
		{
		public:
			D3DFont(void);
			~D3DFont(void);

			ID3DXFont* create(IDirect3DDevice9* device = NULL);
			void destroy(ID3DXFont*& font);
			int drawText(const std::string& text, const RECT& rect, ID3DXFont* font = NULL);
		};//class D3DFont
	}//namespace multimedia
}//namespace framework

#endif//D3D_FONT_H

