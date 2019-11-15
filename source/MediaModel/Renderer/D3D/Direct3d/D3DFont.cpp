#include "error.h"
#include "MediaModel/Renderer/D3D/Direct3d/D3DFont.h"

NS_BEGIN(d3d, 1)

D3DFont::D3DFont()
{}

D3DFont::~D3DFont()
{}

ID3DXFont* D3DFont::create(IDirect3DDevice9* device /* = NULL */)
{
	ID3DXFont* tempOfD3DFont{ NULL };

	if (device)
	{
		if (SUCCEEDED(D3DXCreateFontA(device, 50, 20, 20, 0, FALSE, DEFAULT_CHARSET, 0, 0, 0, "Arial", &tempOfD3DFont)))
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
	return font ? font->DrawTextA(NULL, text.c_str(), static_cast<int>(text.length()), (RECT*)&rect, DT_SINGLELINE | DT_NOCLIP | DT_CENTER | DT_VCENTER, 0xFFFFFF00) : ERR_INVALID_PARAM;
}

NS_END
