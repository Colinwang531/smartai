//
//		Copyright :					@2017, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2019-07-25
//		Description:					����Ƶ��Ͻ������������
//
//		History:						Author									Date														Description
//											������									2019-07-25										����
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
