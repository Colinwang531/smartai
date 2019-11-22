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
#include "MediaModel/Decoder/MediaDecoder.h"

NS_BEGIN(model, 1)

class HikvisionSDKDecoder : public MediaDecoder
{
public:
	HikvisionSDKDecoder(void);
	virtual ~HikvisionSDKDecoder(void);

public:
	int inputMediaData(MediaDataPtr mediaData) override;

private:
	static void CALLBACK postDecodeFrameInfoCallback(
		long decoderID = 0, char* decodeFrame = NULL, long frameBytes = 0, 
		FRAME_INFO* frameInfo = NULL, void* nUser = NULL, void* nReserved2 = NULL);

private:
	long decoderID;
};//class HikvisionSDKDecoder

NS_END

#endif//HIKVISION_SDK_DECODER_H
