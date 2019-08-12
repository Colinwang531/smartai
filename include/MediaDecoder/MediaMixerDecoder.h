//
//		Copyright :					@2017, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2019-07-25
//		Description:					音视频混合解码器抽象基类
//
//		History:						Author									Date														Description
//											王科威									2019-07-25										创建
//

#ifndef MEDIA_MIXER_DECODER_H
#define MEDIA_MIXER_DECODER_H

#include "MediaDecoder.h"

NS_BEGIN(decoder, 1)

class MediaMixerDecoder : public MediaDecoder
{
public:
	MediaMixerDecoder(void);
	virtual ~MediaMixerDecoder(void);

protected:
	//[1, 500]
	long decoderID;
};//class MediaMixerDecoder

NS_END

#endif//MEDIA_MIXER_DECODER_H
