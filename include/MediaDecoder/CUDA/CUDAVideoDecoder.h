//
//		Copyright :					@2019, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2019-09-16
//		Description:					CUDA硬件加速解码器类
//
//		History:						Author									Date												Description
//											王科威									2019-09-16								创建
//

#ifndef CUDA_VIDEO_DECODER_H
#define CUDA_VIDEO_DECODER_H

#include "nvcuvid.h"
#include "MediaDecoder/MediaMixerDecoder.h"

NS_BEGIN(decoder, 1)

class CUDAVideoDecoder : public MediaMixerDecoder
{
public:
	CUDAVideoDecoder(CUcontext ctx = NULL);
	virtual ~CUDAVideoDecoder(void);

public:
	int decode(
		const unsigned char* frameData = NULL, const int frameBytes = 0, const unsigned long long frameNumber = 0) override;

private:
	/**
*   @brief  Callback function to be registered for getting a callback when decoding of sequence starts
*/
	static int CUDAAPI handleVideoSequenceCallback(void* ctx = NULL, CUVIDEOFORMAT* cuVideoFormat = NULL);
	/**
	*   @brief  Callback function to be registered for getting a callback when a decoded frame is ready to be decoded
	*/
	static int CUDAAPI handlePictureDecodeCallback(void* ctx = NULL, CUVIDPICPARAMS* cuvidPictureParams = NULL);
	/**
	*   @brief  Callback function to be registered for getting a callback when a decoded frame is available for display
	*/
	static int CUDAAPI handlePictureDisplayCallback(void* ctx = NULL, CUVIDPARSERDISPINFO* cuvidParserDisplayInfo = NULL);
	/**
*   @brief  This function gets called when a sequence is ready to be decoded. The function also gets called
	when there is format change
*/
	int videoSequenceProcess(CUVIDEOFORMAT* cuVideoFormat = NULL);
	/**
	*   @brief  This function gets called when a picture is ready to be decoded. cuvidDecodePicture is called from this function
	*   to decode the picture
	*/
	int pictureDecodeProcess(CUVIDPICPARAMS* cuvidPictureParams = NULL);
	/**
	*   @brief  This function gets called after a picture is decoded and available for display. Frames are fetched and stored in
		internal buffer
	*/
	int pictureDisplayProcess(CUVIDPARSERDISPINFO* cuvidParserDisplayInfo = NULL);

private:
	CUcontext cudaContext;
	CUvideoctxlock cudaVideoCtxLock;
	CUvideosource cudaVideoSource;
	CUvideoparser cudaVideoParser;
	CUvideodecoder cudaVideoDecoder;
	CUstream cudaVideoStream;
};//class CUDAVideoDecoder

NS_END

#endif//CUDA_VIDEO_DECODER_H
