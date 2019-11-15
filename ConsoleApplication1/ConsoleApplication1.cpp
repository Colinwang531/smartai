// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

// #include <iostream>
// #include "boost/process.hpp"
// 
// //#define CHILD_PROCESS
// 
// int main(int argc, char* argv[])
// {
// #ifdef CHILD_PROCESS
// 	std::cout << "Child process start, ID = [ " << GetCurrentProcessId() << " ]." << std::endl;
// 	std::cout << "Child process name is [ " << argv[0] << " ]." << std::endl;
// 	std::cout << "Child process parameter is [ " << argv[1] << " ]." << std::endl;
// 	std::cout << "Child process stop." << std::endl;
// 
// 	getchar();
// #else
// 	std::cout << "Parent process start, ID = [ " << GetCurrentProcessId() << " ]." << std::endl;
// //	int result{ boost::process::system("./ConsoleApplication1_child.exe", "Nice to meet you, Colin !") };
// //	boost::process::spawn("./ConsoleApplication1_child.exe", "Nice to meet you, Colin !");
// 	boost::process::child c("./ConsoleApplication1_child.exe", "Nice to meet you, Colin !");
// 	c.detach();
// 	std::cout << "Child process exit code is [ " << c.exit_code() << " ]." << std::endl;
// 	std::cout << "Parent process stop." << std::endl;
// 
// 	getchar();
// #endif//CHILD_PROCESS
// 
// 	return 1;
// }

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file


/*
 * Copyright (c) 2017 Jun Zhao
 * Copyright (c) 2017 Kaixuan Liu
 *
 * HW Acceleration API (video decoding) decode sample
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

 /**
  * @file
  * HW-Accelerated decoding example.
  *
  * @example hw_decode.c
  * This example shows how to do HW-accelerated decoding with output
  * frames from the HW video surfaces.
  */

#include <stdio.h>

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/pixdesc.h>
#include <libavutil/hwcontext.h>
#include <libavutil/opt.h>
#include <libavutil/avassert.h>
#include <libavutil/imgutils.h>
}

//#pragma comment(lib, "D:\\captain\\smartai\\lib\\FFmpeg_4_2_1\\avcodec.lib")
//#pragma comment(lib, "D:\\captain\\smartai\\lib\\FFmpeg_4_2_1\\avdevice.lib")
//#pragma comment(lib, "D:\\captain\\smartai\\lib\\FFmpeg_4_2_1\\avfilter.lib")
//#pragma comment(lib, "D:\\captain\\smartai\\lib\\FFmpeg_4_2_1\\avformat.lib")
//#pragma comment(lib, "D:\\captain\\smartai\\lib\\FFmpeg_4_2_1\\avutil.lib")
//#pragma comment(lib, "D:\\captain\\smartai\\lib\\FFmpeg_4_2_1\\postproc.lib")
//#pragma comment(lib, "D:\\captain\\smartai\\lib\\FFmpeg_4_2_1\\swresample.lib")
//#pragma comment(lib, "D:\\captain\\smartai\\lib\\FFmpeg_4_2_1\\swscale.lib")

static AVBufferRef* hw_device_ctx = NULL;
static enum AVPixelFormat hw_pix_fmt;
static FILE* output_file = NULL;

static int hw_decoder_init(AVCodecContext* ctx, const enum AVHWDeviceType type)
{
	int err = 0;

	if ((err = av_hwdevice_ctx_create(&hw_device_ctx, type,
		NULL, NULL, 0)) < 0) {
		fprintf(stderr, "Failed to create specified HW device.\n");
		return err;
	}
	ctx->hw_device_ctx = av_buffer_ref(hw_device_ctx);

	return err;
}

static enum AVPixelFormat get_hw_format(AVCodecContext* ctx,
	const enum AVPixelFormat* pix_fmts)
{
	const enum AVPixelFormat* p;

	for (p = pix_fmts; *p != -1; p++) {
		if (*p == hw_pix_fmt)
			return *p;
	}

	fprintf(stderr, "Failed to get HW surface format.\n");
	return AV_PIX_FMT_NONE;
}

static int decode_write(AVCodecContext* avctx, AVPacket* packet)
{
	AVFrame* frame = NULL, * sw_frame = NULL;
	AVFrame* tmp_frame = NULL;
	uint8_t* buffer = NULL;
	int size;
	int ret = 0;

	ret = avcodec_send_packet(avctx, packet);
	if (ret < 0) {
		fprintf(stderr, "Error during decoding\n");
		return ret;
	}

	while (1) {
		if (!(frame = av_frame_alloc()) || !(sw_frame = av_frame_alloc())) {
			fprintf(stderr, "Can not alloc frame\n");
			ret = AVERROR(ENOMEM);
			goto fail;
		}

		ret = avcodec_receive_frame(avctx, frame);
		if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
			av_frame_free(&frame);
			av_frame_free(&sw_frame);
			return 0;
		}
		else if (ret < 0) {
			fprintf(stderr, "Error while decoding\n");
			goto fail;
		}

		if (frame->format == hw_pix_fmt) {
			/* retrieve data from GPU to CPU */
			if ((ret = av_hwframe_transfer_data(sw_frame, frame, 0)) < 0) {
				fprintf(stderr, "Error transferring the data to system memory\n");
				goto fail;
			}
			tmp_frame = sw_frame;
		}
		else
			tmp_frame = frame;

		size = av_image_get_buffer_size((AVPixelFormat)tmp_frame->format, tmp_frame->width,
			tmp_frame->height, 1);
		buffer = (uint8_t*)av_malloc(size);
		if (!buffer) {
			fprintf(stderr, "Can not alloc buffer\n");
			ret = AVERROR(ENOMEM);
			goto fail;
		}
		ret = av_image_copy_to_buffer(buffer, size,
			(const uint8_t* const*)tmp_frame->data,
			(const int*)tmp_frame->linesize, (AVPixelFormat)tmp_frame->format,
			tmp_frame->width, tmp_frame->height, 1);
		if (ret < 0) {
			fprintf(stderr, "Can not copy image to buffer\n");
			goto fail;
		}

// 		if ((ret = fwrite(buffer, 1, size, output_file)) < 0) {
// 			fprintf(stderr, "Failed to dump raw data.\n");
// 			goto fail;
//		}

	fail:
		av_frame_free(&frame);
		av_frame_free(&sw_frame);
		av_freep(&buffer);
		if (ret < 0)
			return ret;
	}
}

int main(int argc, char* argv[])
{
	AVFormatContext* input_ctx = NULL;
	int video_stream, ret;
	AVStream* video = NULL;
	AVCodecContext* decoder_ctx = NULL;
	AVCodec* decoder = NULL;
	AVPacket packet;
	enum AVHWDeviceType type;
	int i;

	//if (argc < 4) {
	//	fprintf(stderr, "Usage: %s <device type> <input file> <output file>\n", argv[0]);
	//	return -1;
	//}

	type = av_hwdevice_find_type_by_name(argv[1]);
	if (type == AV_HWDEVICE_TYPE_NONE) {
		fprintf(stderr, "Device type %s is not supported.\n", argv[1]);
		fprintf(stderr, "Available device types:");
		while ((type = av_hwdevice_iterate_types(type)) != AV_HWDEVICE_TYPE_NONE)
			fprintf(stderr, " %s", av_hwdevice_get_type_name(type));
		fprintf(stderr, "\n");
		return -1;
	}

	/* open the input file */
	if (avformat_open_input(&input_ctx, argv[2], NULL, NULL) != 0) {
		fprintf(stderr, "Cannot open input file '%s'\n", argv[2]);
		return -1;
	}

	if (avformat_find_stream_info(input_ctx, NULL) < 0) {
		fprintf(stderr, "Cannot find input stream information.\n");
		return -1;
	}

	/* find the video stream information */
	ret = av_find_best_stream(input_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, &decoder, 0);
	if (ret < 0) {
		fprintf(stderr, "Cannot find a video stream in the input file\n");
		return -1;
	}
	video_stream = ret;

	/* find the audio stream information */
	AVCodec* audio = NULL;
	ret = av_find_best_stream(input_ctx, AVMEDIA_TYPE_AUDIO, -1, -1, &audio, 0);
	if (ret < 0) {
		fprintf(stderr, "Cannot find a video stream in the input file\n");
		return -1;
	}

	for (i = 0;; i++) {
		const AVCodecHWConfig* config = avcodec_get_hw_config(decoder, i);
		if (!config) {
			fprintf(stderr, "Decoder %s does not support device type %s.\n",
				decoder->name, av_hwdevice_get_type_name(type));
			return -1;
		}
		if (config->methods & AV_CODEC_HW_CONFIG_METHOD_HW_DEVICE_CTX &&
			config->device_type == type) {
			hw_pix_fmt = config->pix_fmt;
			break;
		}
	}

	if (!(decoder_ctx = avcodec_alloc_context3(decoder)))
		return AVERROR(ENOMEM);

	video = input_ctx->streams[video_stream];
	if (avcodec_parameters_to_context(decoder_ctx, video->codecpar) < 0)
		return -1;

	decoder_ctx->get_format = get_hw_format;

	if (hw_decoder_init(decoder_ctx, type) < 0)
		return -1;

	if ((ret = avcodec_open2(decoder_ctx, decoder, NULL)) < 0) {
		fprintf(stderr, "Failed to open codec for stream #%u\n", video_stream);
		return -1;
	}

	/* open the file to dump raw data */
	output_file = NULL;// fopen(argv[3], "w+");

	/* actual decoding and dump the raw data */
	while (ret >= 0) {
		if ((ret = av_read_frame(input_ctx, &packet)) < 0)
			break;

		if (video_stream == packet.stream_index)
			ret = decode_write(decoder_ctx, &packet);

		av_packet_unref(&packet);
	}

	/* flush the decoder */
	packet.data = NULL;
	packet.size = 0;
	ret = decode_write(decoder_ctx, &packet);
	av_packet_unref(&packet);

	if (output_file)
		fclose(output_file);
	avcodec_free_context(&decoder_ctx);
	avformat_close_input(&input_ctx);
	av_buffer_unref(&hw_device_ctx);

	return 0;
}

