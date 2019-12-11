// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Video and audio decoder filter class.
//

#ifndef AV_DECODER_FILTER_H
#define AV_DECODER_FILTER_H

#include "MediaFilter/MediaFilter.h"

namespace framework
{
	namespace multimedia
	{
		typedef enum class tagDecoderType_t : unsigned char
		{
			DECODER_TYPE_NONE = 0,
			DECODER_TYPE_HIKVISION,
			DECODER_TYPE_DAHUA,
			DECODER_TYPE_H264,
			DECODER_TYPE_H265,
			DECODER_TYPE_RTSP_RTP,
			DECODER_TYPE_AAC
		}DecoderType;

		class AVDecoderFilter : public MediaFilter
		{
		public:
			AVDecoderFilter(
				const DecoderType type = DecoderType::DECODER_TYPE_NONE);
			virtual ~AVDecoderFilter(void);

		protected:
			int createNewFilter(void) override;
			int createNewModule(MediaDataPtr mediaData) override;

		private:
			const DecoderType decoderType;
		};//class AVDecoderFilter
	}//namespace multimedia
}//namespace framework

#endif//AV_DECODER_FILTER_H
