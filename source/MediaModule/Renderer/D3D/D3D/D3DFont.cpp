#include "error.h"
#include "MediaModule/Renderer/D3D/D3D/D3DFont.h"

namespace framework
{
	namespace multimedia
	{
		D3DFont::D3DFont()
		{}

		D3DFont::~D3DFont()
		{}

		ID3DXFont* D3DFont::create(IDirect3DDevice9* device /* = NULL */)
		{
			ID3DXFont* tempOfD3DFont{ NULL };

			if (device)
			{
				if (SUCCEEDED(D3DXCreateFontA(device, 22, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial", &tempOfD3DFont)))
				{
				}
			}

			return tempOfD3DFont;
		}

		void D3DFont::destroy(ID3DXFont*& font)
		{
			if (font)
			{
				font->Release();
				font = NULL;
			}
		}

		int D3DFont::drawText(const std::string& text, const RECT& rect, ID3DXFont* font /* = NULL */)
		{
// 			const char* word = "Arial";
// 			const int len = (int)strlen(word);
// 			D3DXFONT_DESC df;
// 			ZeroMemory(&df, sizeof(D3DXFONT_DESC));
// 			df.Height = 25;
// 			df.Width = 12;
// 			df.MipLevels = D3DX_DEFAULT;
// 			df.Italic = false;
// 			df.CharSet = DEFAULT_CHARSET;
// 			df.OutputPrecision = 0;
// 			df.Quality = 0;
// 			df.PitchAndFamily = 0;
// 			memcpy_s(df.FaceName, len, word, len);
// 
// 			RECT rc;
// 			rc.left = 100;
// 			rc.top = 100;
// 			rc.right = 500;
// 			rc.bottom = 500;

			int status{ font->/*DrawTextA(NULL, "Hello World", -1, &rc, DT_TOP | DT_LEFT, 0xff000000)*/DrawTextA(NULL, text.c_str(), -1, (RECT*)&rect, DT_SINGLELINE | DT_NOCLIP | DT_LEFT | DT_TOP, 0xF5F5F500) };
			return status;
		}
	}//namespace multimedia
}//namespace framework
