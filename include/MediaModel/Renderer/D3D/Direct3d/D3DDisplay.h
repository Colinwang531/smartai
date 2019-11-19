#ifndef d3d_render_h__
#define d3d_render_h__

#include <memory>
#include <windows.h>
#include <d3d9.h>

class D3DTextureImpl
{
public:
	D3DTextureImpl();
	~D3DTextureImpl();

public:
	bool Create(HWND hWnd, size_t rgbWidth, size_t rgbHeight);
	void Destroy();

	bool RenderBGRA(uint8_t *rgb, uint32_t rgbWidth, uint32_t rgbHeight);

private:
	bool ResizeTexture(uint32_t rgbWidth, uint32_t rgbHeight);
	void SetSamplerState();
	bool TestCooperativeLevel(uint32_t rgbWidth, uint32_t rgbHeight);
	bool IsNeedReCreate(size_t rgbWidth, size_t rgbHeight);

private:
	HWND m_hWnd;
	size_t m_rgbWidth;
	size_t m_rgbHeight;

	size_t m_wndWidth;
	size_t m_wndHeight;

	IDirect3D9 *m_d3d;
	IDirect3DDevice9 *m_d3dDevice;
	IDirect3DVertexBuffer9 *m_d3dVertexBuffer;
	IDirect3DTexture9 *m_d3dTexture;
};

#endif // d3d_render_h__