#include "error.h"
#include "MediaConverter/FFmpegConverter.h"

NS_BEGIN(converter, 1)

FFmpegConverter::FFmpegConverter()
	: MediaConverter(), inputAVFrame{ NULL }, outputAVFrame{ NULL }, swsContext{ NULL },
	inputFrameData{ NULL }, outputFrameData{ NULL }
{}

FFmpegConverter::~FFmpegConverter()
{}

int FFmpegConverter::initialize(const unsigned short imageWidth /* = 1920 */, const unsigned short imageHeight /* = 1080 */)
{
	int status{ ERR_INVALID_PARAM };

	if (0 < imageWidth && 0 < imageHeight)
	{
		deinitialize();
		inputAVFrame = av_frame_alloc();
		outputAVFrame = av_frame_alloc();
		//It should be invoked only once.
		swsContext = sws_getContext(
			imageWidth, imageHeight, AV_PIX_FMT_YUV420P,
			imageWidth, imageHeight, AV_PIX_FMT_BGR24,
			SWS_BICUBIC, NULL, NULL, NULL);

		if (inputAVFrame && outputAVFrame && swsContext)
		{
			status = ERR_OK;
		} 
		else
		{
			deinitialize();
			status = ERR_BAD_ALLOC;
		}
	}

	return status;
}

void FFmpegConverter::deinitialize()
{
	av_frame_free(&inputAVFrame);
	av_frame_free(&outputAVFrame);
	sws_freeContext(swsContext);
}

const unsigned char* FFmpegConverter::convert(
	const unsigned char* imageData /* = NULL */, const unsigned long long imageBytes /* = 0 */,
	const unsigned short imageWidth /* = 1920 */, const unsigned short imageHeight /* = 1080 */)
{
	int status{ ERR_OK };

	if (ERR_OK == status && inputAVFrame && outputAVFrame && swsContext)
	{
		sws_scale(
			swsContext, (uint8_t const* const*)inputAVFrame->data, inputAVFrame->linesize, 0,
			imageHeight, outputAVFrame->data, outputAVFrame->linesize);
	}

	return outputFrameData;
}

NS_END
