extern "C"
{
#include "libavutil/imgutils.h"
}
#include "error.h"
#include "MediaScaler/FFmpeg/YV12ToBGR24.h"

NS_BEGIN(scaler, 1)

YV12ToBGR24::YV12ToBGR24()
	: FrameScaler(), yv12FrameData{ NULL }, bgr24FrameData{ NULL }, swsContext{ NULL },
	yv12FrameDataBuffer{ NULL }, bgr24FrameDataBuffer{ NULL }, yuv420PFrameData{ NULL }
{}

YV12ToBGR24::~YV12ToBGR24()
{
	deinitialize();
}

const char* YV12ToBGR24::scale(
	const char* frame /* = NULL */, const int frameBytes /* = 0 */, const int w /* = 0 */, const int h /* = 0 */)
{
	if (!FrameScaler::scale(frame, frameBytes, w, h))
	{
		return NULL;
	}

	int status{ ERR_OK };

	if (!swsContext && !yv12FrameData && !bgr24FrameData)
	{
		status = initialize(w, h);
	}

	if (ERR_OK == status && ERR_OK == convertYV12ToYUV420P(frame, frameBytes, w, h))
	{
		av_image_fill_arrays(
			yv12FrameData->data, yv12FrameData->linesize, (const uint8_t*)yuv420PFrameData, AV_PIX_FMT_YUV420P, w, h, 1);

		status = 0 < sws_scale(
			swsContext, (uint8_t const* const*)yv12FrameData->data, yv12FrameData->linesize, 0,
			h, bgr24FrameData->data, bgr24FrameData->linesize) ? ERR_OK : ERR_BAD_OPERATE;
	}

	return ERR_OK == status ? bgr24FrameDataBuffer : NULL;
}

int YV12ToBGR24::initialize(const int w /* = 0 */, const int h /* = 0 */)
{
	yv12FrameData = av_frame_alloc();
	bgr24FrameData = av_frame_alloc();

	if (yv12FrameData && bgr24FrameData)
	{
		int bgr24FrameDataBytes{ av_image_get_buffer_size(AV_PIX_FMT_BGR24, w, h, 1) };
		bgr24FrameDataBuffer = (char*)av_malloc(bgr24FrameDataBytes);
		int yv12FrameDataBytes = int{ av_image_get_buffer_size(AV_PIX_FMT_YUV420P, w, h, 1) };
		yv12FrameDataBuffer = (char*)av_malloc(yv12FrameDataBytes);

		if (yv12FrameDataBuffer && bgr24FrameDataBuffer)
		{
			av_image_fill_arrays(
				bgr24FrameData->data, bgr24FrameData->linesize, 
				(const uint8_t*)bgr24FrameDataBuffer, AV_PIX_FMT_BGR24, w, h, 1);

			//It should be invoked only once.
			swsContext = sws_getContext(w, h, AV_PIX_FMT_YUV420P, w, h, AV_PIX_FMT_BGR24, SWS_BICUBIC, NULL, NULL, NULL);
			if (!swsContext)
			{
				deinitialize();
			}
		}
	}

	return swsContext ? ERR_OK : ERR_BAD_ALLOC;
}

void YV12ToBGR24::deinitialize()
{
	av_frame_free(&yv12FrameData);
	av_frame_free(&bgr24FrameData);
	av_free(yv12FrameDataBuffer);
	av_free(bgr24FrameDataBuffer);
	av_free(yuv420PFrameData);
	sws_freeContext(swsContext);
}

int YV12ToBGR24::convertYV12ToYUV420P(
	const char* data /* = NULL */, const int dataBytes /* = 0 */, const int w /* = 1920 */, const int h /* = 1080 */)
{
	if (!yuv420PFrameData)
	{
		int pixelBufferBytes{ av_image_get_buffer_size(AV_PIX_FMT_YUV420P, w, h, 1) };
		yuv420PFrameData = (char*)av_malloc(pixelBufferBytes);
	}

	int yStrideBytes = w * h;
	int uvStrideBytes = yStrideBytes / 4;
	int srcUOffset = yStrideBytes;
	int srcVOffset = srcUOffset + uvStrideBytes;
	int dstVOffset = yStrideBytes;
	int dstUOffset = dstVOffset + uvStrideBytes;

	memcpy(yuv420PFrameData, data, yStrideBytes);
	memcpy(yuv420PFrameData + dstVOffset, data + srcVOffset, uvStrideBytes);
	memcpy(yuv420PFrameData + dstUOffset, data + srcUOffset, uvStrideBytes);

	return ERR_OK;
}

NS_END
