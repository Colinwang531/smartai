#include "error.h"
#include "boost/make_shared.hpp"
#include "MediaData/MediaData.h"

NS_BEGIN(media, 1)

MediaData::MediaData(
	const MediaDataMainID mainID /* = MediaDataMainID::MEDIA_DATA_MAIN_ID_NONE */,
	const MediaDataSubID subID /* = MediaDataSubID::MEDIA_DATA_SUB_ID_NONE */,
	const MediaDataPatchID pacthID /* = MediaDataPatchID::MEDIA_DATA_PATCH_ID_NONE */)
	: mediaDataMainID(mainID), mediaDataSubID{ subID }, mediaDataPatchID{ pacthID }, mediaDataBytes{ 0 }, mediaDataRaw{ NULL }, mediaData{ NULL }, copyMediaData{ false }
{}

MediaData::~MediaData()
{
	if (mediaData && copyMediaData)
	{
		delete[] mediaData;
	}
}

int MediaData::copyData(const unsigned char* data /* = NULL */, const unsigned long long dataBytes /* = 0 */)
{
	int status{ data && 0 < dataBytes ? ERR_OK : ERR_INVALID_PARAM };

	if (ERR_OK == status)
	{
		mediaData = new(std::nothrow) unsigned char[dataBytes];
		if (mediaData)
		{
			memcpy_s(mediaData, dataBytes, data, dataBytes);
			mediaDataBytes = dataBytes;
			copyMediaData = true;
		}
		status = mediaData ? ERR_OK : ERR_BAD_ALLOC;
	}

	return status;
}

int MediaData::setData(const unsigned char* data /* = NULL */, const unsigned long long dataBytes /* = 0 */)
{
	int status{ data && 0 < dataBytes ? ERR_OK : ERR_INVALID_PARAM };
	
	if (ERR_OK == status)
	{
		mediaData = (unsigned char*)data;
		mediaDataBytes = dataBytes;
	}

	return status;
}

NS_END
