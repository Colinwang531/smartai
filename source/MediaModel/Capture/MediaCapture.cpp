#include "error.h"
#include "MediaData/MediaData.h"
#include "MediaModel/Capture/MediaCapture.h"

namespace framework
{
	namespace multimedia
	{
		MediaCapture::MediaCapture() : MediaModel()
		{}

		MediaCapture::~MediaCapture()
		{}

		int MediaCapture::inputMediaData(MediaDataPtr mediaData)
		{
			return mediaData ? openStream((const char*)(mediaData->getData())) : ERR_INVALID_PARAM;
		}
	}
}
