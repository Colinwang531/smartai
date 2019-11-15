extern "C"
{
#include "libavutil/imgutils.h"
}
#include "error.h"
#include "MediaConverter/BGR24ToYUV420PConverter.h"

NS_BEGIN(converter, 1)

BGR24ToYUV420PConverter::BGR24ToYUV420PConverter() 
	: FFmpegConverter(AV_PIX_FMT_BGR24, AV_PIX_FMT_YUV420P)
{}

BGR24ToYUV420PConverter::~BGR24ToYUV420PConverter()
{}

int BGR24ToYUV420PConverter::initialize(
	const unsigned short imageWidth /* = 1920 */, const unsigned short imageHeight /* = 1080 */)
{
	int status{ FFmpegConverter::initialize(imageWidth, imageHeight) };

	if (ERR_OK == status)
	{
		int bgr24ImageBytes{ av_image_get_buffer_size(AV_PIX_FMT_BGR24, imageWidth, imageHeight, 1) };
		inputFrameData = reinterpret_cast<unsigned char*>(av_malloc(bgr24ImageBytes));
		int yuv420pImageBytes{ av_image_get_buffer_size(AV_PIX_FMT_YUV420P, imageWidth, imageHeight, 1) };
		outputFrameData = reinterpret_cast<unsigned char*>(av_malloc(yuv420pImageBytes));

		if (inputFrameData && outputFrameData)
		{
			av_image_fill_arrays(
				outputAVFrame->data, outputAVFrame->linesize, (const uint8_t*)outputFrameData,
				AV_PIX_FMT_YUV420P, imageWidth, imageHeight, 1);
		}
		else
		{
			deinitialize();
			status = ERR_BAD_ALLOC;
		}
	}

	return status;
}

void BGR24ToYUV420PConverter::deinitialize()
{
	FFmpegConverter::deinitialize();
	av_free(inputFrameData);
	av_free(outputFrameData);
}

const unsigned char* BGR24ToYUV420PConverter::convert(
	const unsigned char* imageData /* = NULL */, const unsigned long long imageBytes /* = 0 */,
	const unsigned short imageWidth /* = 1920 */, const unsigned short imageHeight /* = 1080 */)
{
	if (imageData && 0 < imageBytes)
	{
		av_image_fill_arrays(
			inputAVFrame->data, inputAVFrame->linesize, (const uint8_t*)imageData, 
			AV_PIX_FMT_BGR24, imageWidth, imageHeight, 1);
	}

	return FFmpegConverter::convert(imageData, imageBytes, imageWidth, imageHeight);
}

NS_END
