#include <memory>
#include "error.h"
#include "MediaData/MediaData.h"

namespace framework
{
	namespace multimedia
	{
		MediaData::MediaData(
			const MediaDataMainID mainID /* = MediaDataMainID::MEDIA_DATA_MAIN_ID_NONE */,
			const MediaDataSubID subID /* = MediaDataSubID::MEDIA_DATA_SUB_ID_NONE */,
			const MediaDataPatchID pacthID /* = MediaDataPatchID::MEDIA_DATA_PATCH_ID_NONE */)
			: mediaDataMainID(mainID), mediaDataSubID{ subID }, mediaDataPatchID{ pacthID },
			mediaDataBytes{ 0 }, mediaData{ NULL }, persistence{ false }, imageWidth{ 0 }, imageHeight{ 0 }, userData{ NULL }
		{}

		MediaData::~MediaData()
		{
			if (mediaData && persistence)
			{
				delete[] mediaData;
			}
		}

		int MediaData::setData(
			const unsigned char* data /* = NULL */, const unsigned long long dataBytes /* = 0 */, const bool persist /* = false */)
		{
			int status{ data && 0 < dataBytes ? ERR_OK : ERR_INVALID_PARAM };

			if (ERR_OK == status)
			{
				if (persist)
				{
					mediaData = new(std::nothrow) unsigned char[dataBytes];
					if (mediaData)
					{
						memcpy_s(mediaData, dataBytes, data, dataBytes);
						mediaDataBytes = dataBytes;
						persistence = true;
					}
				}
				else
				{
					mediaData = const_cast<unsigned char*>(data);
					mediaDataBytes = dataBytes;
				}
			}

			return status;
		}
	}//namespace multimedia
}//namespace framework
