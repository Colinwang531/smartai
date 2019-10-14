//
//		Copyright :					@2019, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2019-10-02
//		Description:					帧基类
//
//		History:						Author									Date														Description
//											王科威									2019-10-02										创建
//

#ifndef MEDIA_FRAME_H
#define MEDIA_FRAME_H

#include "predef.h"

NS_BEGIN(frame, 1)

typedef enum class tagMediaImageType_t
{
	MEDIA_IMAGE_NONE = 0,
	MEDIA_IMAGE_H264,
	MEDIA_IMAGE_YV12,
	MEDIA_IMAGE_YUV420P,
	MEDIA_IMAGE_BGR24,
	MEDIA_IMAGE_JPEG
}MediaImageType;

class MediaImage
{
public:
	MediaImage(
		const MediaImageType& type = MediaImageType::MEDIA_IMAGE_NONE);
	~MediaImage(void);

public:
	int setOriginImage(
		const unsigned char* data = NULL, const unsigned long long dataBytes = 0);
	int setImage(
		const unsigned char* data = NULL, const unsigned long long dataBytes = 0);
	void setImageParams(
		const unsigned short width = 1920, const unsigned short height = 1080);
	inline const unsigned char* getImage(void) const
	{
		return imageData;
	}
	inline const long long getImageBytes(void) const
	{
		return imageBytes;
	}
	inline const unsigned char* getOriginImage(void) const
	{
		return originImageData;
	}
	inline const long long getOriginImageBytes(void) const
	{
		return originImageBytes;
	}
	inline const MediaImageType& getImageType(void) const
	{
		return imageType;
	}
	inline const unsigned short getImageWidth(void) const
	{
		return imageWidth;
	}
	inline const unsigned short getImageHeight(void) const
	{
		return imageHeight;
	}

private:
	void cleanup(void);

private:
	unsigned char* imageData;
	unsigned long long imageBytes;
	unsigned char* originImageData;
	unsigned long long originImageBytes;
	const MediaImageType imageType;
	unsigned short imageWidth;
	unsigned short imageHeight;
};//class MediaImage

NS_END

#endif//MEDIA_FRAME_H
