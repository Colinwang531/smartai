#include <memory>
extern "C"
{
#include "libavutil/imgutils.h"
}
#include "boost/checked_delete.hpp"
#include "error.h"
#include "MediaEncoder/FFmpegEncoder.h"

NS_BEGIN(encoder, 1)

FFmpegEncoder::FFmpegEncoder()
	: MediaEncoder(), outputImageData{ NULL }, outputImageDataBytes{ 0 }, codec{ NULL }, ctx{ NULL }
{}

FFmpegEncoder::~FFmpegEncoder()
{}

int FFmpegEncoder::initialize(
	const unsigned short imageWidth /* = 1920 */, const unsigned short imageHeight /* = 1080 */)
{
	int status{ ERR_EXISTED };

	if (!ctx && codec)
	{
		ctx = avcodec_alloc_context3(codec);
		ctx->pix_fmt = AV_PIX_FMT_YUVJ420P;
		ctx->codec_type = AVMEDIA_TYPE_VIDEO;
		ctx->time_base.num = 1;
		ctx->time_base.den = 11;
		ctx->width = imageWidth;
		ctx->height = imageHeight;

		status = !avcodec_open2(ctx, codec, NULL) ? ERR_OK : ERR_BAD_OPERATE;
	}

	return status;
}

void FFmpegEncoder::deinitialize()
{
	avcodec_close(ctx);
	av_free(codec);
	boost::checked_array_delete(outputImageData);
}

int FFmpegEncoder::encode(
	const unsigned char* imageData /* = NULL */, const unsigned long long imageBytes /* = 0 */, 
	const unsigned short imageWidth /* = 1920 */, const unsigned short imageHeight /* = 1080 */)
{
	int status{ ERR_BAD_ALLOC };
	boost::checked_array_delete(outputImageData);
	outputImageData = new(std::nothrow) unsigned char[imageBytes + 1];

	if (outputImageData)
	{
		outputImageData[imageBytes] = 0;

#ifdef _WINDOWS
		memcpy_s(outputImageData, imageBytes, imageData, imageBytes);
#else
		memcpy(outputImageData, imageData, imageBytes);
#endif//_WINDOWS
		
		outputImageDataBytes = imageBytes;
		status = ERR_OK;
	}

	return status;
}

void FFmpegEncoder::data(unsigned char*& outputData, unsigned long long& outputBytes)
{
	outputData = outputImageData;
	outputBytes = outputImageDataBytes;
}

NS_END
