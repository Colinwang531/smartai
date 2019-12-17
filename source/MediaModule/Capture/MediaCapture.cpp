#include "error.h"
#include "MediaModule/Capture/MediaCapture.h"

namespace framework
{
	namespace multimedia
	{
		MediaCapture::MediaCapture() : MediaModule()
		{}

		MediaCapture::~MediaCapture()
		{}

		int MediaCapture::inputMediaData(MediaDataPtr mediaData)
		{
			return ERR_NOT_SUPPORT;
		}
	}//namespace multimedia
}//namespace framework
