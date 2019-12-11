#include "error.h"
#include "MediaModule/Renderer/D3D/D3D/D3DVertexBuffer.h"
#include "MediaModule/Renderer/D3D/D3D/D3DDevice.h"

namespace framework
{
	namespace multimedia
	{
		static const DWORD gFunctionCodeOf420ps30[] =
		{
			0xffff0300, 0x003ffffe, 0x42415443, 0x0000001c, 0x000000c7, 0xffff0300,
			0x00000004, 0x0000001c, 0x20000102, 0x000000c0, 0x0000006c, 0x00010003,
			0x00000001, 0x00000074, 0x00000000, 0x00000084, 0x00020003, 0x00000001,
			0x00000074, 0x00000000, 0x0000008c, 0x00000003, 0x00000001, 0x00000074,
			0x00000000, 0x00000094, 0x00000002, 0x00000001, 0x000000a0, 0x000000b0,
			0x78655455, 0x00657574, 0x000c0004, 0x00010001, 0x00000001, 0x00000000,
			0x78655456, 0x00657574, 0x78655459, 0x00657574, 0x6e617274, 0x72617073,
			0x00746e65, 0x00030000, 0x00010001, 0x00000001, 0x00000000, 0x3f800000,
			0x00000000, 0x00000000, 0x00000000, 0x335f7370, 0x4d00305f, 0x6f726369,
			0x74666f73, 0x29522820, 0x534c4820, 0x6853204c, 0x72656461, 0x6d6f4320,
			0x656c6970, 0x2e392072, 0x392e3432, 0x322e3934, 0x00373033, 0x05000051,
			0xa00f0001, 0x3f950a81, 0x3fcc4a9d, 0xbf5fcbb4, 0x00000000, 0x05000051,
			0xa00f0002, 0x3f950a81, 0xbec89507, 0xbf501eac, 0x3f081b65, 0x05000051,
			0xa00f0003, 0x3f950a81, 0x40011a54, 0xbf8af5f5, 0x00000000, 0x05000051,
			0xa00f0004, 0x3f800000, 0x00000000, 0x00000000, 0x00000000, 0x0200001f,
			0x80000005, 0x90030000, 0x0200001f, 0x80010005, 0x90030001, 0x0200001f,
			0x80020005, 0x90030002, 0x0200001f, 0x90000000, 0xa00f0800, 0x0200001f,
			0x90000000, 0xa00f0801, 0x0200001f, 0x90000000, 0xa00f0802, 0x03000042,
			0x800f0000, 0x90e40002, 0xa0e40802, 0x02000001, 0x80040000, 0x80000000,
			0x03000042, 0x800f0001, 0x90e40000, 0xa0e40800, 0x04000004, 0x80090000,
			0x80000001, 0xa0640004, 0xa0250004, 0x03000008, 0x80010800, 0xa0e40001,
			0x80f80000, 0x03000042, 0x800f0001, 0x90e40001, 0xa0e40801, 0x02000001,
			0x80020000, 0x80000001, 0x03000009, 0x80020800, 0xa0e40002, 0x80e40000,
			0x03000008, 0x80040800, 0xa0e40003, 0x80f40000, 0x02000001, 0x80080800,
			0xa0000000, 0x0000ffff
		};

		static const DWORD gFunctionCodeOf420ps20[] =
		{
			0xffff0200, 0x003ffffe, 0x42415443, 0x0000001c, 0x000000c7, 0xffff0200,
			0x00000004, 0x0000001c, 0x20000102, 0x000000c0, 0x0000006c, 0x00010003,
			0x00000001, 0x00000074, 0x00000000, 0x00000084, 0x00020003, 0x00000001,
			0x00000074, 0x00000000, 0x0000008c, 0x00000003, 0x00000001, 0x00000074,
			0x00000000, 0x00000094, 0x00000002, 0x00000001, 0x000000a0, 0x000000b0,
			0x78655455, 0x00657574, 0x000c0004, 0x00010001, 0x00000001, 0x00000000,
			0x78655456, 0x00657574, 0x78655459, 0x00657574, 0x6e617274, 0x72617073,
			0x00746e65, 0x00030000, 0x00010001, 0x00000001, 0x00000000, 0x3f800000,
			0x00000000, 0x00000000, 0x00000000, 0x325f7370, 0x4d00305f, 0x6f726369,
			0x74666f73, 0x29522820, 0x534c4820, 0x6853204c, 0x72656461, 0x6d6f4320,
			0x656c6970, 0x2e392072, 0x392e3432, 0x322e3934, 0x00373033, 0x05000051,
			0xa00f0001, 0x3f950a81, 0x00000000, 0x3fcc4a9d, 0xbf5fcbb4, 0x05000051,
			0xa00f0002, 0x3f950a81, 0xbec89507, 0xbf501eac, 0x3f081b65, 0x05000051,
			0xa00f0003, 0x3f950a81, 0x40011a54, 0x00000000, 0xbf8af5f5, 0x05000051,
			0xa00f0004, 0x3f800000, 0x00000000, 0x00000000, 0x00000000, 0x0200001f,
			0x80000000, 0xb0030000, 0x0200001f, 0x80000000, 0xb0030001, 0x0200001f,
			0x80000000, 0xb0030002, 0x0200001f, 0x90000000, 0xa00f0800, 0x0200001f,
			0x90000000, 0xa00f0801, 0x0200001f, 0x90000000, 0xa00f0802, 0x03000042,
			0x800f0000, 0xb0e40000, 0xa0e40800, 0x03000042, 0x800f0001, 0xb0e40001,
			0xa0e40801, 0x03000042, 0x800f0002, 0xb0e40002, 0xa0e40802, 0x02000001,
			0x80080003, 0xa0000000, 0x02000001, 0x80020000, 0x80000001, 0x02000001,
			0x80040000, 0x80000002, 0x02000001, 0x80080000, 0xa0000004, 0x03000009,
			0x80010003, 0xa0e40001, 0x80e40000, 0x03000009, 0x80020003, 0xa0e40002,
			0x80e40000, 0x03000009, 0x80040003, 0xa0e40003, 0x80e40000, 0x02000001,
			0x800f0800, 0x80e40003, 0x0000ffff
		};

		D3DDevice::D3DDevice()
		{}

		D3DDevice::~D3DDevice()
		{}

		IDirect3DDevice9* D3DDevice::create(
			IDirect3D9* d3d9 /* = NULL */, const HWND hwnd /* = NULL */, const int width /* = 0 */, const int height /* = 0 */)
		{
			IDirect3DDevice9* tempOfD3DDevice9{ NULL };

			if (d3d9)
			{
				D3DPRESENT_PARAMETERS tempOfD3DPP{ 0 };
				tempOfD3DPP.hDeviceWindow = const_cast<HWND>(hwnd);
				tempOfD3DPP.BackBufferWidth = width;
				tempOfD3DPP.BackBufferHeight = height;
				tempOfD3DPP.BackBufferFormat = D3DFMT_UNKNOWN;
				tempOfD3DPP.BackBufferCount = 2;
				tempOfD3DPP.Windowed = TRUE;
				tempOfD3DPP.SwapEffect = D3DSWAPEFFECT_DISCARD;
				D3DCAPS9 tempOfD3DCaps9{};
				DWORD tempOfBehaviorFlags{ D3DCREATE_SOFTWARE_VERTEXPROCESSING };

				if (SUCCEEDED(d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &tempOfD3DCaps9)))
				{
					if (tempOfD3DCaps9.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
					{
						tempOfBehaviorFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING;
					}

					if (SUCCEEDED(d3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, tempOfD3DPP.hDeviceWindow, tempOfBehaviorFlags, (D3DPRESENT_PARAMETERS*)&tempOfD3DPP, &tempOfD3DDevice9)))
					{
					}
				}
			}

			return tempOfD3DDevice9;
		}

		void D3DDevice::destroy(IDirect3DDevice9*& device)
		{
			if (device)
			{
				device->Release();
				device = NULL;
			}
		}

		DWORD* D3DDevice::getFunctionCode(IDirect3DDevice9* device /*= NULL*/)
		{
			DWORD* tempOfFunctionCode = NULL;

			if (device)
			{
				D3DCAPS9 tempOfD3DCaps9{};
				if (SUCCEEDED(device->GetDeviceCaps(&tempOfD3DCaps9)))
				{
					tempOfFunctionCode = const_cast<DWORD*>(tempOfD3DCaps9.PixelShaderVersion >= D3DPS_VERSION(3, 0) ? gFunctionCodeOf420ps30 : gFunctionCodeOf420ps20);
				}
			}

			return tempOfFunctionCode;
		}

		int D3DDevice::setPixelShader(IDirect3DDevice9* device /* = NULL */, IDirect3DPixelShader9* shader /* = NULL */)
		{
			int status{ ERR_BAD_OPERATE };

			if (SUCCEEDED(device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX3)) && SUCCEEDED(device->SetPixelShader(shader)))
			{
				status = ERR_OK;
			}

			return status;
		}

		int D3DDevice::setTexture(IDirect3DDevice9* device /* = NULL */, ID3DXConstantTable* constant /* = NULL */, IDirect3DTexture9** textures /* = NULL */)
		{
			int status{ ERR_INVALID_PARAM };

			if (device && constant && textures)
			{
				const char* const tempOfNameForTexture[] =
				{
					"YTextue", "UTextue", "VTextue"
				};

				UINT tempOfCountNumber{ 0 };
				D3DXCONSTANT_DESC tempOfD3DXConstantDesc{};

				for (int i = 0; i != 3; ++i)
				{
					D3DXHANDLE tempOfD3DXHandle{ constant->GetConstantByName(NULL, tempOfNameForTexture[i]) };

					if (tempOfD3DXHandle && SUCCEEDED(constant->GetConstantDesc(tempOfD3DXHandle, &tempOfD3DXConstantDesc, &tempOfCountNumber)) &&
						SUCCEEDED(device->SetTexture(tempOfD3DXConstantDesc.RegisterIndex, textures[i])))
					{
					}
					else
					{
						status = ERR_BAD_OPERATE;
						break;
					}
				}
			}

			return status;
		}

		int D3DDevice::present(IDirect3DDevice9* device /* = NULL */, IDirect3DVertexBuffer9* vertexBuffer /* = NULL */)
		{
			int status{ ERR_BAD_OPERATE };

			if (SUCCEEDED(device->BeginScene()) && SUCCEEDED(device->SetStreamSource(0, vertexBuffer, 0, sizeof(D3DVertex))) &&
				SUCCEEDED(device->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2)) && SUCCEEDED(device->EndScene()))
			{
				if (SUCCEEDED(device->Present(NULL, NULL, NULL, NULL)))
				{
					status = ERR_OK;
				}
			}

			return status;
		}
	}//namespace multimedia
}//namespace framework
