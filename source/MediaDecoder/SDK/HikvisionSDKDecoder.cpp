#include "error.h"
#include "MediaDecoder/SDK/HikvisionSDKDecoder.h"

NS_BEGIN(decoder, 1)

HikvisionSDKDecoder::HikvisionSDKDecoder(FrameDataDecodeHandler handler /* = NULL */)
	: MediaMixerDecoder(), frameDataDecodeHandler{ handler }
{}

HikvisionSDKDecoder::~HikvisionSDKDecoder(void)
{
	if (-1 < decoderID)
	{
		PlayM4_Stop(decoderID);
		PlayM4_CloseStream(decoderID);
		PlayM4_FreePort(decoderID);
		frameDataDecodeHandler = NULL;
	}
}

int HikvisionSDKDecoder::decode(
	const unsigned char* frameData /* = NULL */, const int frameBytes /* = 0 */, const unsigned long long frameNumber /* = 0 */)
{
	int status{ MediaDecoder::decode(frameData, frameBytes, frameNumber) };

	if (ERR_OK == status)
	{
		if (-1 < decoderID)
		{
			status = PlayM4_InputData(decoderID, const_cast<BYTE*>(frameData), frameBytes) ? ERR_OK : ERR_BAD_OPERATE;
		}
		else
		{
			if (PlayM4_GetPort(&decoderID) && -1 < decoderID)
			{
				PlayM4_SetStreamOpenMode(decoderID, STREAME_REALTIME);
				PlayM4_OpenStream(decoderID, const_cast<BYTE*>(frameData), frameBytes, 3145728);
				PlayM4_SetDecCallBackExMend(decoderID, &HikvisionSDKDecoder::decodeFrameInfoCallback, NULL, 0, this);
				//HARD_DECODE_ENGINE / SOFT_DECODE_ENGINE
				PlayM4_SetDecodeEngineEx(decoderID, SOFT_DECODE_ENGINE);
//				PlayM4_SetDecodeFrameType(decoderID, 5);
				status = PlayM4_Play(decoderID, NULL) ? ERR_OK : ERR_BAD_OPERATE;
			}
			else
			{
				status = ERR_BAD_OPERATE;
			}
		}
	}

	return status;
}

void HikvisionSDKDecoder::decodeFrameInfoCallback(
	long decoderID /* = 0 */, char* decodeFrame /* = NULL */, long frameBytes /* = 0 */, 
	FRAME_INFO* frameInfo /* = NULL */, void* nUser /* = NULL */, void* nReserved2 /* = NULL */)
{
	HikvisionSDKDecoder* decoder{ reinterpret_cast<HikvisionSDKDecoder*>(nUser) };

	if (decoder && decoder->frameDataDecodeHandler)
	{
		if (T_YV12 == frameInfo->nType)
		{
			decoder->frameDataDecodeHandler(
				decodeFrame, frameBytes, 0, DECODE_FRAME_VIDEO, frameInfo->nWidth, frameInfo->nHeight);
		}
		else if (T_AUDIO16 == frameInfo->nType)
		{
		}
	}
}

NS_END
