// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Base class of decoder model using HIKVISION library.
//

#ifndef HIKVISION_SDK_DECODER_H
#define HIKVISION_SDK_DECODER_H

#include "WindowsPlayM4.h"
#include "MediaModule/MediaModule.h"

namespace framework
{
	namespace multimedia
	{
		class HikvisionSDKDecoder : public MediaModule
		{
		public:
			HikvisionSDKDecoder(void);
			virtual ~HikvisionSDKDecoder(void);

		public:
			int inputMediaData(MediaDataPtr mediaData) override;

		private:
			static void CALLBACK postSDKDecodeFrameCallback(
				long decoderID = 0, char* decodeFrame = NULL, long frameBytes = 0,
				FRAME_INFO* frameInfo = NULL, void* nUser = NULL, void* nReserved2 = NULL);

		private:
			long decoderID;
		};//class HikvisionSDKDecoder
	}//namespace multimedia
}//namespace framework

#endif//HIKVISION_SDK_DECODER_H
