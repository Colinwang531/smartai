#include "error.h"
#include "MediaModel/Renderer/D3D/D3D/D3DTexture.h"

namespace framework
{
	namespace multimedia
	{
		D3DTexture::D3DTexture()
		{}

		D3DTexture::~D3DTexture()
		{}

		IDirect3DTexture9* D3DTexture::create(IDirect3DDevice9* device /* = NULL */, const int width /* = 0 */, const int height /* = 0 */)
		{
			IDirect3DTexture9* tempOfD3DTexture9{ NULL };

			if (device && 0 < width && 0 < height)
			{
				if (SUCCEEDED(device->CreateTexture(width, height, 1, D3DUSAGE_DYNAMIC, D3DFMT_L8, D3DPOOL_DEFAULT, &tempOfD3DTexture9, NULL)))
				{
				}
			}

			return tempOfD3DTexture9;
		}

		void D3DTexture::destroy(IDirect3DTexture9* texture)
		{
			if (texture)
			{
				texture->Release();
				texture = NULL;
			}
		}

		int D3DTexture::drawImage(
			IDirect3DTexture9* texture /* = NULL */, const char* data /* = NULL */, const int width /* = 0 */, const int height /* = 0 */)
		{
			if (!texture || !data || !width || !height)
			{
				return ERR_INVALID_PARAM;
			}

			int status{ ERR_BAD_OPERATE };
			D3DLOCKED_RECT tempOfLockedRect{};

			if (SUCCEEDED(texture->LockRect(0, &tempOfLockedRect, NULL, D3DLOCK_DISCARD)))
			{
				const int tempOfWidthPlusHeight{ width * height };

				if (tempOfLockedRect.Pitch == width)
				{
#if defined(_WIN32) || defined(_WIN64)
					memcpy_s(tempOfLockedRect.pBits, tempOfWidthPlusHeight, data, tempOfWidthPlusHeight);
#else
					memcpy(tempOfLockedRect.pBits, data, tempOfWidthPlusHeight);
#endif
				}
				else
				{
					int tempOfPosition{ 0 }, tempOfTimes{ 0 };

					for (int i = 0; i != height; ++i)
					{
#if defined(_WIN32) || defined(_WIN64)
						memcpy_s((char*)tempOfLockedRect.pBits + tempOfPosition, width, data + tempOfTimes * width, width);
#else
						memcpy(tempOfLockedRect.pBits, data, tempOfWidthPlusHeight);
#endif
						tempOfPosition += tempOfLockedRect.Pitch;
						++tempOfTimes;
					}
				}

				texture->UnlockRect(0);
				status = ERR_OK;
			}

			return status;
		}
	}//namespace multimedia
}//namespace framework
