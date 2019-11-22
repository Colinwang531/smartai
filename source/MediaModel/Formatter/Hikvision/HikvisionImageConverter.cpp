extern "C"
{
#include "libavutil/imgutils.h"
}
#include "boost/make_shared.hpp"
#include "error.h"
#include "MediaConverter/YV12ToYUV420PConverter.h"
#include "MediaConverter/Hikvision/HikvisionImageConverter.h"

NS_BEGIN(converter, 1)

HikvisionImageConverter::HikvisionImageConverter() : YUV420PToBGR24Converter()
{}

HikvisionImageConverter::~HikvisionImageConverter()
{}

int HikvisionImageConverter::initialize(
	const unsigned short imageWidth /* = 1920 */, const unsigned short imageHeight /* = 1080 */)
{
	int status{ YUV420PToBGR24Converter::initialize(imageWidth, imageHeight) };

	if (ERR_OK == status)
	{
		boost::shared_ptr<MediaConverter> mediaConverterPtr{ 
			boost::make_shared<YV12ToYUV420PConverter>() };
		if (mediaConverterPtr)
		{
			yv12ToYUV420PConverterPtr.swap(mediaConverterPtr);
			status = yv12ToYUV420PConverterPtr->initialize(imageWidth, imageHeight);
		}
		else
		{
			status = ERR_BAD_ALLOC;
		}
	}

	return status;
}

void HikvisionImageConverter::deinitialize()
{
	yv12ToYUV420PConverterPtr->deinitialize();
	YUV420PToBGR24Converter::deinitialize();
}

const unsigned char* HikvisionImageConverter::convert(
	const unsigned char* imageData /* = NULL */, const unsigned long long imageBytes /* = 0 */,
	const unsigned short imageWidth /* = 1920 */, const unsigned short imageHeight /* = 1080 */)
{
	const int yuv420PImageBytes{ 
		av_image_get_buffer_size(AV_PIX_FMT_YUV420P, imageWidth, imageHeight, 1) };
	const unsigned char* yuv420PImageData{ 
		yv12ToYUV420PConverterPtr->convert(imageData, imageBytes, imageWidth, imageHeight) };

	return yuv420PImageData ? YUV420PToBGR24Converter::convert(yuv420PImageData, yuv420PImageBytes, imageWidth, imageHeight) : NULL;
}

NS_END
