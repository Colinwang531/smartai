#include "error.h"
#include "boost/checked_delete.hpp"
#include "boost/make_shared.hpp"
#include "MediaData/MediaData.h"

NS_BEGIN(media, 1)

MediaData::MediaData(const MediaDataType& type /* = MediaDataType::MEDIA_DATA_TYPE_NONE */)
	: mediaDataType(type), mediaDataPtr{ NULL }, mediaDataBytes{ 0 }
{}

MediaData::~MediaData()
{}

void MediaData::resetMediaDataType(const MediaDataType& type /* = MediaDataType::MEDIA_DATA_TYPE_NONE */)
{
	mediaDataType = type;
}

void MediaData::resetMediaDataBuffer()
{
	mediaDataPtr.reset();
	mediaDataBytes = 0;
}

int MediaData::setMediaDataBuffer(const char* dataBuffer /* = NULL */, const unsigned long long dataBytes /* = 0 */)
{
	int status{ ERR_INVALID_PARAM };
	
	if (dataBuffer && 0 < dataBytes)
	{
		resetMediaDataBuffer();
		mediaDataPtr = boost::make_shared<char[]>(dataBytes, 0);
		if (mediaDataPtr)
		{
#ifdef _WINDOWS
			memcpy_s(mediaDataPtr.get(), mediaDataBytes, dataBuffer, dataBytes);
#else
			memcpy(mediaDataPtr.get(), dataBuffer, dataBytes);
#endif//WINDOWS
		}
		mediaDataBytes = dataBytes;
	}

	return mediaDataPtr && 0 < mediaDataBytes ? ERR_OK : ERR_BAD_ALLOC;
}

NS_END
