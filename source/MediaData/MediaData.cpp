#include "error.h"
#include "boost/make_shared.hpp"
#include "MediaData/MediaData.h"

NS_BEGIN(media, 1)

MediaData::MediaData(
	const MediaDataMainID mainID /* = MediaDataMainID::MEDIA_DATA_MAIN_ID_NONE */,
	const MediaDataSubID subID /* = MediaDataSubID::MEDIA_DATA_SUB_ID_NONE */,
	const MediaDataPatchID pacthID /* = MediaDataPatchID::MEDIA_DATA_PATCH_ID_NONE */)
	: mediaDataMainID(mainID), mediaDataSubID{ subID }, mediaDataPatchID{ pacthID }, mediaDataBytes{ 0 }, mediaDataRaw{ NULL }, mediaData{ NULL }
{}

MediaData::~MediaData()
{}

int MediaData::setData(const unsigned char* data /* = NULL */, const unsigned long long dataBytes /* = 0 */)
{
	int status{ data && 0 < dataBytes ? ERR_OK : ERR_INVALID_PARAM };
	
// 	if (ERR_OK == status && !mediaDataPtr)
// 	{
// 		mediaDataBytes = dataBytes;
// 		mediaDataPtr = boost::make_shared<unsigned char[]>(dataBytes, 0);
// 
// 		if (mediaDataPtr)
// 		{
// #ifdef _WINDOWS
// 			memcpy_s(mediaDataPtr.get(), mediaDataBytes, data, dataBytes);
// #else
// 			memcpy(mediaDataPtr.get(), data, dataBytes);
// #endif//WINDOWS
// 		}
// 		else
// 		{
// 			status = ERR_BAD_ALLOC;
// 		}
// 	}
// 	else
// 	{
// 		status = ERR_BAD_OPERATE;
// 	}
	mediaData = (unsigned char*)data;
	mediaDataBytes = dataBytes;

	return status;
}

NS_END
