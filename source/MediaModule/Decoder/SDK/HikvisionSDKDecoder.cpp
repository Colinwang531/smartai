#include "boost/make_shared.hpp"
#include "error.h"
#include "MediaData/MediaData.h"
#include "MediaModel/Decoder/SDK/HikvisionSDKDecoder.h"

namespace framework
{
	namespace multimedia
	{
		HikvisionSDKDecoder::HikvisionSDKDecoder() : MediaDecoder(), decoderID{ -1 }
		{}

		HikvisionSDKDecoder::~HikvisionSDKDecoder(void)
		{
			if (-1 < decoderID)
			{
				PlayM4_Stop(decoderID);
				PlayM4_CloseStream(decoderID);
				PlayM4_FreePort(decoderID);
			}
		}

		int HikvisionSDKDecoder::inputMediaData(MediaDataPtr mediaData)
		{
			int status{ ERR_OK };
			unsigned char* frameData{ const_cast<unsigned char*>(mediaData->getData()) };
			long frameBytes{ static_cast<long>(mediaData->getDataBytes()) };

			if (-1 < decoderID)
			{
				status = PlayM4_InputData(decoderID, frameData, frameBytes) ? ERR_OK : ERR_BAD_OPERATE;
			}
			else
			{
				if (PlayM4_GetPort(&decoderID) && -1 < decoderID)
				{
					PlayM4_SetStreamOpenMode(decoderID, STREAME_REALTIME);
					PlayM4_SetDecCallBackExMend(decoderID, &HikvisionSDKDecoder::postSDKDecodeFrameCallback, NULL, 0, this);
					PlayM4_OpenStream(decoderID, frameData, frameBytes, 1048576);
					//HARD_DECODE_ENGINE / SOFT_DECODE_ENGINE
					PlayM4_SetDecodeEngineEx(decoderID, SOFT_DECODE_ENGINE);
					status = PlayM4_Play(decoderID, NULL) ? ERR_OK : ERR_BAD_OPERATE;
				}
				else
				{
					status = ERR_BAD_OPERATE;
				}
			}

			return status;
		}

		void HikvisionSDKDecoder::postSDKDecodeFrameCallback(
			long decoderID /* = 0 */, char* decodeFrame /* = NULL */, long frameBytes /* = 0 */,
			FRAME_INFO* frameInfo /* = NULL */, void* nUser /* = NULL */, void* nReserved2 /* = NULL */)
		{
			HikvisionSDKDecoder* hikvisionSdkDecoder{ reinterpret_cast<HikvisionSDKDecoder*>(nUser) };

			if (hikvisionSdkDecoder && hikvisionSdkDecoder->postInputMediaDataCallback)
			{
				if (T_YV12 == frameInfo->nType)
				{
					MediaDataPtr mediaDataPtr{
					boost::make_shared<MediaData>(
						MediaDataMainID::MEDIA_DATA_MAIN_ID_VIDEO, MediaDataSubID::MEDIA_DATA_SUB_ID_YV12) };
					if (mediaDataPtr)
					{
						mediaDataPtr->setData((unsigned char*)decodeFrame, frameBytes);
						mediaDataPtr->setImageParameter(frameInfo->nWidth, frameInfo->nHeight);
						hikvisionSdkDecoder->postInputMediaDataCallback(mediaDataPtr);
					}
				}
				else if (T_AUDIO16 == frameInfo->nType)
				{
				}
			}
		}
	}//namespace multimedia
}//namespace framework
