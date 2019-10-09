#include <memory>
#include "boost/checked_delete.hpp"
#include "error.h"
#include "MediaFrame/MediaFrame.h"

NS_BEGIN(frame, 1)

MediaImage::MediaImage(const MediaImageType& type /* = MediaImageType::MEDIA_IMAGE_NONE */)
	: imageData{ NULL }, originImageData{ NULL }, imageBytes{ 0 }, originImageBytes{ 0 },
	imageType{ type }, imageWidth{ 0 }, imageHeight{ 0 }
{}

MediaImage::~MediaImage()
{
	//Self check delete.
	cleanup();
}

int MediaImage::setImage(
	const unsigned char* data /* = NULL */, const unsigned long long dataBytes /* = 0 */)
{
	int status{ ERR_INVALID_PARAM };

	if (data && 0 < dataBytes && !imageData)
	{
		imageData = new(std::nothrow) unsigned char[dataBytes];
		if (imageData)
		{
			imageBytes = dataBytes;
#ifdef _WINDOWS
			memcpy_s(imageData, imageBytes, data, dataBytes);
#else
			memcpy(imageData, data, dataBytes);
#endif//_WINDOWS

			status = ERR_OK;
		}
		else
		{
			status = ERR_BAD_ALLOC;
		}
	}

	return status;
}

int MediaImage::setOriginImage(
	const unsigned char* data /* = NULL */, const unsigned long long dataBytes /* = 0 */)
{
	int status{ ERR_INVALID_PARAM };

	if (data && 0 < dataBytes && !originImageData)
	{
		originImageData = new(std::nothrow) unsigned char[dataBytes];
		if (originImageData)
		{
			originImageBytes = dataBytes;
#ifdef _WINDOWS
			memcpy_s(originImageData, originImageBytes, data, dataBytes);
#else
			memcpy(originImageData, data, dataBytes);
#endif//_WINDOWS

			status = ERR_OK;
		}
		else
		{
			status = ERR_BAD_ALLOC;
		}
	}

	return status;
}

void MediaImage::setImageParams(
	const unsigned short width /* = 1920 */, const unsigned short height /* = 1080 */)
{
	imageWidth = width;
	imageHeight = height;
}

void MediaImage::cleanup()
{
	boost::checked_array_delete(imageData);
	boost::checked_array_delete(originImageData);
}

NS_END
