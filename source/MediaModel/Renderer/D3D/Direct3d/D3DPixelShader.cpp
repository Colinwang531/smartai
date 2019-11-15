#include "d3dx9shader.h"
#include "MediaModel/Renderer/D3D/Direct3d/D3DDevice.h"
#include "MediaModel/Renderer/D3D/Direct3d/D3DPixelShader.h"

NS_BEGIN(d3d, 1)

D3DPixelShader::D3DPixelShader()
{}

D3DPixelShader::~D3DPixelShader()
{}

IDirect3DPixelShader9* D3DPixelShader::create(IDirect3DDevice9* device /* = NULL */)
{
	IDirect3DPixelShader9* tempOfD3DPixelShader9{ NULL };
	DWORD* tempOfFunctionCode{ D3DDevice().getFunctionCode(device) };

	if (device && tempOfFunctionCode)
	{
		if (SUCCEEDED(device->CreatePixelShader(tempOfFunctionCode, &tempOfD3DPixelShader9)))
		{
		}
	}

	return tempOfD3DPixelShader9;
}

void D3DPixelShader::destroy(IDirect3DPixelShader9*& shader)
{
	if (shader)
	{
		shader->Release();
		shader = NULL;
	}
}

ID3DXConstantTable* D3DPixelShader::getShaderConstantTable(IDirect3DDevice9* device /* = NULL */)
{
	ID3DXConstantTable* tempOfD3DXConstantTable{ NULL };
	DWORD* tempOfFunctionCode{ D3DDevice().getFunctionCode(device) };

	if (device && tempOfFunctionCode)
	{
		if (SUCCEEDED(D3DXGetShaderConstantTable(tempOfFunctionCode, &tempOfD3DXConstantTable)) && 
			SUCCEEDED(tempOfD3DXConstantTable->SetDefaults(device)))
		{
		}
	}

	return tempOfD3DXConstantTable;
}

NS_END
