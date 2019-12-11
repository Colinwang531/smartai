#include "MediaModule/Renderer/D3D/D3D/D3DDisplay.h"
//#include <glog/logger.h>

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_TEX1)

struct D3dCustomVertex {
	float x, y, z;
	float u, v;
};

D3DTextureImpl::D3DTextureImpl()
	: m_hWnd(NULL)
	, m_rgbWidth(0)
	, m_rgbHeight(0)
	, m_wndWidth(0)
	, m_wndHeight(0)
	, m_d3d(NULL)
	, m_d3dDevice(NULL)
	, m_d3dVertexBuffer(NULL)
	, m_d3dTexture(NULL)
{
}

D3DTextureImpl::~D3DTextureImpl()
{
	Destroy();
}

bool D3DTextureImpl::Create(HWND hWnd, size_t rgbWidth, size_t rgbHeight)
{
	do
	{
		Destroy();

		if (NULL == hWnd)
		{
//			LOG_INFO << "hWnd is null";
			break;
		}
		else
		{
			m_hWnd = hWnd;
		}

		m_d3d = Direct3DCreate9(D3D_SDK_VERSION);
		if (nullptr == m_d3d)
		{
//			LOG_DEBUG << "Direct3DCreate9 faild";
			break;
		}

		D3DPRESENT_PARAMETERS d3d_params = {};
		d3d_params.Windowed = TRUE;
		d3d_params.SwapEffect = D3DSWAPEFFECT_COPY;

		IDirect3DDevice9* d3d_device = NULL;
		HRESULT hRet = m_d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
										   m_hWnd,
										   D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
										   &d3d_params,
										   &d3d_device);
		if (D3D_OK != hRet)
		{
//			LOG_DEBUG << "CreateDevice faild:" << hRet;
			break;
		}
		m_d3dDevice = d3d_device;

		IDirect3DVertexBuffer9* vertex_buffer = NULL;
		const int kRectVertices = 4;
		hRet = m_d3dDevice->CreateVertexBuffer(kRectVertices * sizeof(D3dCustomVertex),
											   0,
											   D3DFVF_CUSTOMVERTEX,
											   D3DPOOL_MANAGED,
											   &vertex_buffer,
											   NULL);
		if (D3D_OK != hRet)
		{
//			LOG_DEBUG << "CreateVertexBuffer faild:" << hRet;
			break;
		}
		m_d3dVertexBuffer = vertex_buffer;

		if (!ResizeTexture(rgbWidth, rgbHeight))
		{
//			LOG_DEBUG << "ResizeTexture faild";
			break;
		}

		m_d3dDevice->Present(NULL, NULL, NULL, NULL);

		RECT rtWnd;
		::GetClientRect(m_hWnd, &rtWnd);
		size_t wndWidth = abs(rtWnd.right - rtWnd.left);
		size_t wndHeight = abs(rtWnd.bottom - rtWnd.top);

		m_rgbWidth = rgbWidth;
		m_rgbHeight = rgbHeight;
		m_wndWidth = wndWidth;
		m_wndHeight = wndHeight;

//		LOG_INFO << "D3DTexture Create success";
		return true;
	} while (0);

	Destroy();
	return false;
}

void D3DTextureImpl::Destroy()
{
	if (m_d3dTexture)
	{
		m_d3dTexture->Release();
		m_d3dTexture = NULL;
	}

	if (m_d3dVertexBuffer)
	{
		m_d3dVertexBuffer->Release();
		m_d3dVertexBuffer = NULL;
	}

	if (m_d3dDevice)
	{
		m_d3dDevice->Release();
		m_d3dDevice = NULL;
	}

	if (m_d3d)
	{
		m_d3d->Release();
		m_d3d = NULL;
	}
}

bool D3DTextureImpl::RenderBGRA(uint8_t *rgb, uint32_t rgbWidth, uint32_t rgbHeight)
{
	if (IsNeedReCreate(rgbWidth, rgbHeight))
	{
		if (!Create(m_hWnd, rgbWidth, rgbHeight))
		{
//			LOG_INFO << "Create faild";
			return false;
		}
	}

	if (!TestCooperativeLevel(rgbWidth, rgbHeight))
	{
//		LOG_INFO << "TestCooperativeLevel faild";
		return false;
	}

	D3DLOCKED_RECT lock_rect;
	HRESULT hr = m_d3dTexture->LockRect(0, &lock_rect, NULL, 0);
	if (hr != D3D_OK)
	{
//		LOG_INFO << "LockRect faild:" << hr;
		return false;
	}

	//to do copy
	char * pDest = reinterpret_cast<char*>(lock_rect.pBits);
	const char * pSrc = reinterpret_cast<const char*>(rgb);
	int stride = lock_rect.Pitch;
	int pixel_w_size = rgbWidth * 32 / 8;

	for (uint32_t i = 0; i < rgbHeight; i++)
	{
		memcpy(pDest, pSrc, pixel_w_size);
		pDest += stride;
		pSrc += pixel_w_size;
	}

	m_d3dTexture->UnlockRect(0);

	//draw
	m_d3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(100, 100, 100), 1.0f, 0);
	m_d3dDevice->BeginScene();
	m_d3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
	m_d3dDevice->SetStreamSource(0, m_d3dVertexBuffer, 0, sizeof(D3dCustomVertex));
	m_d3dDevice->SetTexture(0, m_d3dTexture);//启用纹理
	m_d3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);//利用索引缓存配合顶点缓存绘制图形
	m_d3dDevice->EndScene();

	m_d3dDevice->Present(NULL, NULL, NULL, NULL);
	return true;
}

bool D3DTextureImpl::ResizeTexture(uint32_t rgbWidth, uint32_t rgbHeight)
{
	if (m_d3dTexture)
	{
		m_d3dTexture->Release();
		m_d3dTexture = NULL;
	}

	IDirect3DTexture9* texture = NULL;

	m_d3dDevice->CreateTexture(static_cast<UINT>(rgbWidth),
							   static_cast<UINT>(rgbHeight),
							   1,
							   0,
							   D3DFMT_A8R8G8B8,
							   D3DPOOL_MANAGED,
							   &texture,
							   NULL);
	if (nullptr == texture)
	{
//		LOG_INFO << "CreateTexture texture is null";
		return false;
	}
	else
	{
		m_d3dTexture = texture;
	}

	// Vertices for the video frame to be rendered to.
	static const D3dCustomVertex rect[] =
	{
		{ -1.0f, -1.0f, 0.0f, 0.0f, 1.0f },
		{ -1.0f, 1.0f, 0.0f, 0.0f, 0.0f },
		{ 1.0f, -1.0f, 0.0f, 1.0f, 1.0f },
		{ 1.0f, 1.0f, 0.0f, 1.0f, 0.0f },
	};

	void* buf_data;
	HRESULT hr = m_d3dVertexBuffer->Lock(0, 0, &buf_data, 0);
	if (hr != D3D_OK)
	{
//		LOG_INFO << "m_d3dVertexBuffer Lock faild:" << hr;
		return false;
	}

	memcpy(buf_data, &rect, sizeof(rect));
	m_d3dVertexBuffer->Unlock();

	SetSamplerState();

	return true;
}

void D3DTextureImpl::SetSamplerState()
{
	IDirect3DDevice9_SetVertexShader(m_d3dDevice, NULL);
	IDirect3DDevice9_SetFVF(m_d3dDevice, D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);
	IDirect3DDevice9_SetRenderState(m_d3dDevice, D3DRS_ZENABLE, D3DZB_FALSE);
	IDirect3DDevice9_SetRenderState(m_d3dDevice, D3DRS_CULLMODE, D3DCULL_NONE);
	IDirect3DDevice9_SetRenderState(m_d3dDevice, D3DRS_LIGHTING, FALSE);

	/* Enable color modulation by diffuse color */
	IDirect3DDevice9_SetTextureStageState(m_d3dDevice, 0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	IDirect3DDevice9_SetTextureStageState(m_d3dDevice, 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	IDirect3DDevice9_SetTextureStageState(m_d3dDevice, 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	/* Enable alpha modulation by diffuse alpha */
	IDirect3DDevice9_SetTextureStageState(m_d3dDevice, 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	IDirect3DDevice9_SetTextureStageState(m_d3dDevice, 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	IDirect3DDevice9_SetTextureStageState(m_d3dDevice, 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	/* Enable separate alpha blend function, if possible */
	IDirect3DDevice9_SetRenderState(m_d3dDevice, D3DRS_SEPARATEALPHABLENDENABLE, TRUE);

	/* Disable second texture stage, since we're done */
	IDirect3DDevice9_SetTextureStageState(m_d3dDevice, 1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	IDirect3DDevice9_SetTextureStageState(m_d3dDevice, 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

	/* Set an identity world and view matrix */
	D3DMATRIX matrix;
	matrix.m[0][0] = 1.0f;
	matrix.m[0][1] = 0.0f;
	matrix.m[0][2] = 0.0f;
	matrix.m[0][3] = 0.0f;
	matrix.m[1][0] = 0.0f;
	matrix.m[1][1] = 1.0f;
	matrix.m[1][2] = 0.0f;
	matrix.m[1][3] = 0.0f;
	matrix.m[2][0] = 0.0f;
	matrix.m[2][1] = 0.0f;
	matrix.m[2][2] = 1.0f;
	matrix.m[2][3] = 0.0f;
	matrix.m[3][0] = 0.0f;
	matrix.m[3][1] = 0.0f;
	matrix.m[3][2] = 0.0f;
	matrix.m[3][3] = 1.0f;
	IDirect3DDevice9_SetTransform(m_d3dDevice, D3DTS_WORLD, &matrix);
	IDirect3DDevice9_SetTransform(m_d3dDevice, D3DTS_VIEW, &matrix);

	//设置Mipmap纹理采样器参数
	int SamplerIdx = 0;
	m_d3dDevice->SetTexture(SamplerIdx, m_d3dTexture);
	m_d3dDevice->SetSamplerState(SamplerIdx, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	m_d3dDevice->SetSamplerState(SamplerIdx, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR);
	m_d3dDevice->SetSamplerState(SamplerIdx, D3DSAMP_ADDRESSW, D3DTADDRESS_WRAP);
	m_d3dDevice->SetSamplerState(SamplerIdx, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_d3dDevice->SetSamplerState(SamplerIdx, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
	m_d3dDevice->SetSamplerState(SamplerIdx, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
	m_d3dDevice->SetSamplerState(SamplerIdx, D3DSAMP_MIPMAPLODBIAS, 1);// Mipmap层级向高精度偏移1个层级
	m_d3dDevice->SetSamplerState(SamplerIdx, D3DSAMP_MAXANISOTROPY, 4);// 最大异性采样阈值设置为4
}

bool D3DTextureImpl::TestCooperativeLevel(uint32_t rgbWidth, uint32_t rgbHeight)
{
	if (NULL == m_d3dDevice)
	{
		return false;
	}

	HRESULT hState = m_d3dDevice->TestCooperativeLevel();

	switch (hState)
	{
	case D3DERR_DEVICELOST://设备丢失
		return false;

	case D3DERR_DEVICENOTRESET://设备可以恢复
		m_wndHeight = 0;
		m_wndWidth = 0;
		return Create(m_hWnd, rgbWidth, rgbHeight);

	case  D3D_OK:
		break;

	default:
		return false;
	}

	return true;
}

bool D3DTextureImpl::IsNeedReCreate(size_t rgbWidth, size_t rgbHeight)
{
	RECT rtWnd;
	::GetClientRect(m_hWnd, &rtWnd);
	size_t wndWidth = abs(rtWnd.right - rtWnd.left);
	size_t wndHeight = abs(rtWnd.bottom - rtWnd.top);

	if ((m_rgbWidth == rgbWidth)
		&& (m_rgbHeight == rgbHeight)
		&& (m_wndWidth == wndWidth)
		&& (m_wndHeight == wndHeight)
		&& (nullptr != m_d3d)
		&& (nullptr != m_d3dDevice))
	{
		return false;
	}

	m_rgbWidth = rgbWidth;
	m_rgbHeight = rgbHeight;
	m_wndWidth = wndWidth;
	m_wndHeight = wndHeight;

	return true;
}