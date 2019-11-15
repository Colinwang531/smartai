//
//		Copyright :					@2017, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2017-12-11
//		Description:					�����豸SDK����Ƶ��Ͻ�������
//
//		History:						Author									Date												Description
//											������									2017-12-11								����
//

#ifndef HIKVISION_SDK_DECODER_H
#define HIKVISION_SDK_DECODER_H

#include "boost/function.hpp"
#include "WindowsPlayM4.h"
#include "MediaDecoder/MediaMixerDecoder.h"

NS_BEGIN(decoder, 1)

typedef boost::function<
	void(const char*, const long, const long, const long)> FrameDataDecodeHandler;

class HikvisionSDKDecoder : public MediaMixerDecoder
{
public:
	HikvisionSDKDecoder(FrameDataDecodeHandler handler = NULL);
	virtual ~HikvisionSDKDecoder(void);

public:
	int decode(
		const unsigned char* frameData = NULL, const int frameBytes = 0) override;

private:
	static void CALLBACK decodeFrameInfoCallback(
		long decoderID = 0, char* decodeFrame = NULL, long frameBytes = 0, 
		FRAME_INFO* frameInfo = NULL, void* nUser = NULL, void* nReserved2 = NULL);

private:
	FrameDataDecodeHandler frameDataDecodeHandler;
};//class HikvisionSDKDecoder

NS_END

#endif//HIKVISION_SDK_DECODER_H
