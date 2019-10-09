extern "C"
{
#include "libavutil/imgutils.h"
}
#include "error.h"
#include "MediaConverter/YV12ToYUV420PConverter.h"

NS_BEGIN(converter, 1)

YV12ToYUV420PConverter::YV12ToYUV420PConverter() : MediaConverter(), outputFrameData{ NULL }
{}

YV12ToYUV420PConverter::~YV12ToYUV420PConverter()
{
	deinitialize();
}

int YV12ToYUV420PConverter::initialize(
	const unsigned short imageWidth /* = 1920 */, const unsigned short imageHeight /* = 1080 */)
{
	int yuv420pImageBytes{ 
		av_image_get_buffer_size(AV_PIX_FMT_YUV420P, imageWidth, imageHeight, 1) };
	outputFrameData = reinterpret_cast<unsigned char*>(av_malloc(yuv420pImageBytes));

	return outputFrameData ? ERR_OK : ERR_BAD_ALLOC;
}

void YV12ToYUV420PConverter::deinitialize()
{
	av_free(outputFrameData);
}

const unsigned char* YV12ToYUV420PConverter::convert(
	const unsigned char* imageData /* = NULL */, const unsigned long long imageBytes /* = 0 */,
	const unsigned short imageWidth /* = 1920 */, const unsigned short imageHeight /* = 1080 */)
{
	if (outputFrameData)
	{
		int yStrideBytes = imageWidth * imageHeight;
		int uvStrideBytes = yStrideBytes / 4;
		int srcUOffset = yStrideBytes;
		int srcVOffset = srcUOffset + uvStrideBytes;
		int dstVOffset = yStrideBytes;
		int dstUOffset = dstVOffset + uvStrideBytes;

#ifdef _WINDOWS
		memcpy_s(outputFrameData, yStrideBytes, imageData, yStrideBytes);
		memcpy_s(outputFrameData + dstVOffset, uvStrideBytes, imageData + srcVOffset, uvStrideBytes);
		memcpy_s(outputFrameData + dstUOffset, uvStrideBytes, imageData + srcUOffset, uvStrideBytes);
#else
		memcpy(outputFrameData, imageData, yStrideBytes);
		memcpy(outputFrameData + dstVOffset, imageData + srcVOffset, uvStrideBytes);
		memcpy(outputFrameData + dstUOffset, imageData + srcUOffset, uvStrideBytes);
#endif//_WINDOWS
	}

	return outputFrameData;
}

NS_END
