extern "C"
{
#include "libavformat/avformat.h"
}
#include "cuda.h"
#include "error.h"
#include "MediaDecoder/CUDA/CUDAVideoDecoder.h"

NS_BEGIN(decoder, 1)

CUDAVideoDecoder::CUDAVideoDecoder(CUcontext ctx /* = NULL */)
	: MediaMixerDecoder(), cudaContext{ ctx }, cudaVideoCtxLock{ NULL },
	cudaVideoSource{ NULL }, cudaVideoParser{ NULL }, cudaVideoDecoder{ NULL }, cudaVideoStream{ NULL }
{}

CUDAVideoDecoder::~CUDAVideoDecoder(void)
{}

int CUDAVideoDecoder::decode(
	const unsigned char* frameData /* = NULL */, const int frameBytes /* = 0 */)
{
	if (!cudaVideoParser && !cudaVideoCtxLock)
	{
		CUVIDPARSERPARAMS videoParserParameters{};
		videoParserParameters.CodecType = cudaVideoCodec_H264;
		videoParserParameters.ulMaxNumDecodeSurfaces = 1;
		videoParserParameters.ulMaxDisplayDelay = 0;// bLowLatency ? 0 : 1;
		videoParserParameters.pUserData = this;
		videoParserParameters.pfnSequenceCallback = &CUDAVideoDecoder::handleVideoSequenceCallback;
		videoParserParameters.pfnDecodePicture = &CUDAVideoDecoder::handlePictureDecodeCallback;
		videoParserParameters.pfnDisplayPicture = &CUDAVideoDecoder::handlePictureDisplayCallback;
		cuvidCreateVideoParser(&cudaVideoParser, &videoParserParameters);
		cuvidCtxLockCreate(&cudaVideoCtxLock, cudaContext);
	}

	if (cudaVideoParser)
	{
		AVCodecParameters* avCodecParameters{ NULL };
		AVFormatContext* avFormatCtx{ NULL };
		const AVBitStreamFilter* avBitStreamFilter{ av_bsf_get_by_name("h264_mp4toannexb") };
		AVBSFContext* avBSFContext{ NULL };

		if (!avformat_open_input(&avFormatCtx, (const char*)frameData, NULL, NULL))
		{
			if (!avformat_find_stream_info(avFormatCtx, NULL))
			{
				AVPacket avpacket, avTargetPacket;
				av_init_packet(&avpacket);
				av_init_packet(&avTargetPacket);
				int videoType{ av_find_best_stream(avFormatCtx, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0) };
				avCodecParameters = avFormatCtx->streams[videoType]->codecpar;
				av_bsf_alloc(avBitStreamFilter, &avBSFContext);
				avcodec_parameters_copy(avBSFContext->par_in, avCodecParameters);
				av_bsf_init(avBSFContext);

				while (1)
				{
					//Only read video stream.
					int status{ av_read_frame(avFormatCtx, &avpacket) };
					if (!status && avpacket.stream_index == videoType)
					{
						if (avTargetPacket.data)
						{
							av_packet_unref(&avTargetPacket);
						}
						av_bsf_send_packet(avBSFContext, &avpacket);
						av_bsf_receive_packet(avBSFContext, &avTargetPacket);

						CUVIDSOURCEDATAPACKET cuvidSourceDataPacket{ 0 };
						cuvidSourceDataPacket.flags |= CUVID_PKT_TIMESTAMP;
						cuvidSourceDataPacket.payload_size = avTargetPacket.size;
						cuvidSourceDataPacket.payload = avTargetPacket.data;
						cuvidSourceDataPacket.timestamp = avTargetPacket.pts;
						cuvidParseVideoData(cudaVideoParser, &cuvidSourceDataPacket);
					}
					else
					{
						av_packet_unref(&avpacket);
					}
				}

				av_packet_unref(&avpacket);
				av_packet_unref(&avTargetPacket);
			}
		}

		av_bsf_free(&avBSFContext);
		avformat_close_input(&avFormatCtx);
		cuvidDestroyVideoParser(cudaVideoParser);
		cuvidCtxLockDestroy(cudaVideoCtxLock);
	}

	return 0;
}

int CUDAVideoDecoder::handleVideoSequenceCallback(void* ctx /* = NULL */, CUVIDEOFORMAT* cuVideoFormat /* = NULL */)
{
	return ((CUDAVideoDecoder*)ctx)->videoSequenceProcess(cuVideoFormat);
}

int CUDAVideoDecoder::handlePictureDecodeCallback(void* ctx /* = NULL */, CUVIDPICPARAMS* cuvidPictureParams /* = NULL */)
{
	return ((CUDAVideoDecoder*)ctx)->pictureDecodeProcess(cuvidPictureParams);
}

int CUDAVideoDecoder::handlePictureDisplayCallback(void* ctx /* = NULL */, CUVIDPARSERDISPINFO* cuvidParserDisplayInfo /* = NULL */)
{
	return ((CUDAVideoDecoder*)ctx)->pictureDisplayProcess(cuvidParserDisplayInfo);
}

int CUDAVideoDecoder::videoSequenceProcess(CUVIDEOFORMAT* cuVideoFormat /*= NULL*/)
{
	CUVIDDECODECREATEINFO cuvidDecoderCreateInfo{ 0 };
	cuvidDecoderCreateInfo.ulWidth = cuVideoFormat->coded_width;
	cuvidDecoderCreateInfo.ulHeight = cuVideoFormat->coded_height;
	cuvidDecoderCreateInfo.ulNumDecodeSurfaces = 20;
	cuvidDecoderCreateInfo.CodecType = cudaVideoCodec_H264;
	cuvidDecoderCreateInfo.ChromaFormat = cuVideoFormat->chroma_format;
	cuvidDecoderCreateInfo.ulCreationFlags = cudaVideoCreate_PreferCUVID;
	cuvidDecoderCreateInfo.bitDepthMinus8 = cuVideoFormat->bit_depth_luma_minus8;
	cuvidDecoderCreateInfo.ulIntraDecodeOnly = 0;
	cuvidDecoderCreateInfo.ulMaxWidth = cuVideoFormat->coded_width;
	cuvidDecoderCreateInfo.ulMaxHeight = cuVideoFormat->coded_height;
	cuvidDecoderCreateInfo.OutputFormat = cudaVideoSurfaceFormat_NV12;
	cuvidDecoderCreateInfo.DeinterlaceMode = cudaVideoDeinterlaceMode_Weave;
	cuvidDecoderCreateInfo.ulTargetWidth = cuVideoFormat->coded_width;
	cuvidDecoderCreateInfo.ulTargetHeight = cuVideoFormat->coded_height;
	cuvidDecoderCreateInfo.ulNumOutputSurfaces = 2;
	cuvidDecoderCreateInfo.vidLock = cudaVideoCtxLock;

	cuCtxPushCurrent(cudaContext);
	CUresult result = cuvidCreateDecoder(&cudaVideoDecoder, &cuvidDecoderCreateInfo);
	cuCtxPopCurrent(NULL);
	return 1;
}

int CUDAVideoDecoder::pictureDecodeProcess(CUVIDPICPARAMS* cuvidPictureParams /*= NULL*/)
{
	cuvidDecodePicture(cudaVideoDecoder, cuvidPictureParams);
	return 1;
}

int CUDAVideoDecoder::pictureDisplayProcess(CUVIDPARSERDISPINFO* cuvidParserDisplayInfo /*= NULL*/)
{
	CUVIDPROCPARAMS videoProcessingParameters{0};
	videoProcessingParameters.progressive_frame = cuvidParserDisplayInfo->progressive_frame;
	videoProcessingParameters.second_field = cuvidParserDisplayInfo->repeat_first_field + 1;
	videoProcessingParameters.top_field_first = cuvidParserDisplayInfo->top_field_first;
	videoProcessingParameters.unpaired_field = cuvidParserDisplayInfo->repeat_first_field < 0;
	videoProcessingParameters.output_stream = cudaVideoStream;

	CUdeviceptr dpSrcFrame{ 0 };
	unsigned int nSrcPitch{ 0 };
	CUresult result{ cuvidMapVideoFrame(cudaVideoDecoder, cuvidParserDisplayInfo->picture_index, &dpSrcFrame, &nSrcPitch, &videoProcessingParameters) };

	CUVIDGETDECODESTATUS DecodeStatus;
	memset(&DecodeStatus, 0, sizeof(DecodeStatus));
	result = cuvidGetDecodeStatus(cudaVideoDecoder, cuvidParserDisplayInfo->picture_index, &DecodeStatus);
	if (result == CUDA_SUCCESS && (DecodeStatus.decodeStatus == cuvidDecodeStatus_Error || DecodeStatus.decodeStatus == cuvidDecodeStatus_Error_Concealed))
	{
		printf("Decode Error occurred for picture %d\n", cuvidParserDisplayInfo->picture_index/*m_nPicNumInDecodeOrder[pDispInfo->picture_index]*/);
	}

	uint8_t* pDecodedFrame = new uint8_t[1920 * 1080 * 3];
	cuCtxPushCurrent(cudaContext);
	CUDA_MEMCPY2D m = { 0 };
	m.srcMemoryType = CU_MEMORYTYPE_DEVICE;
	m.srcDevice = dpSrcFrame;
	m.srcPitch = nSrcPitch;
	m.dstMemoryType = /*m_bUseDeviceFrame ? CU_MEMORYTYPE_DEVICE : */CU_MEMORYTYPE_HOST;
	m.dstDevice = (CUdeviceptr)(m.dstHost = pDecodedFrame);
	m.dstPitch = 1920;// m_nDeviceFramePitch ? m_nDeviceFramePitch : m_nWidth * m_nBPP;
	m.WidthInBytes = 1920;// m_nWidth* m_nBPP;
	m.Height = 1080;// m_nLumaHeight;
	cuMemcpy2DAsync(&m, cudaVideoStream);

	m.srcDevice = (CUdeviceptr)((uint8_t*)dpSrcFrame + m.srcPitch * 1088/*m_nSurfaceHeight*/);
	m.dstDevice = (CUdeviceptr)(m.dstHost = pDecodedFrame + m.dstPitch * 1080/*m_nLumaHeight*/);
	m.Height = 1080/*m_nChromaHeight*/;
	cuMemcpy2DAsync(&m, cudaVideoStream);

		// 	if (m_nNumChromaPlanes == 2)
		// 	{
		// 		m.srcDevice = (CUdeviceptr)((uint8_t*)dpSrcFrame + m.srcPitch * m_nSurfaceHeight * 2);
		// 		m.dstDevice = (CUdeviceptr)(m.dstHost = pDecodedFrame + m.dstPitch * m_nLumaHeight * 2);
		// 		m.Height = m_nChromaHeight;
		// 		CUDA_DRVAPI_CALL(cuMemcpy2DAsync(&m, m_cuvidStream));
		// 	}
	cuStreamSynchronize(cudaVideoStream);
	cuCtxPopCurrent(NULL);

// 	if ((int)m_vTimestamp.size() < m_nDecodedFrame) {
// 		m_vTimestamp.resize(m_vpFrame.size());
// 	}
// 	m_vTimestamp[m_nDecodedFrame - 1] = pDispInfo->timestamp;

	cuvidUnmapVideoFrame(cudaVideoDecoder, dpSrcFrame);

// 	static FILE* f{ NULL };
// 	if (!f)
// 	{
// 		fopen_s(&f, "d:\\decodeFile.yuv", "wb+");
// 	}
// 	fwrite(pDecodedFrame, 1920 * 1080 * 3, 1, f);
	delete[] pDecodedFrame;

	return 1;
}

NS_END
