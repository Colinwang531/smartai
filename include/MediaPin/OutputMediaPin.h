// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Base class of outputting pin.
//

#ifndef OUTPUT_MEDIA_PIN_H
#define OUTPUT_MEDIA_PIN_H

#include "DataStruct/Vector.h"
#include "MediaPin.h"

namespace framework
{
	namespace multimedia
	{
		class OutputMediaPin : public MediaPin
		{
		public:
			OutputMediaPin(void);
			virtual ~OutputMediaPin(void);

		public:
			int inputMediaData(MediaDataPtr mediaData) override;

		protected:
			int connectPin(const MediaPinRef pin) override;
			bool isOutputPin(void) const override
			{
				return false;
			}

		private:
			Vector<MediaPinRef> inputPinGroup;
		};//class OutputMediaPin
	}//namespace multimedia
}//namespace framework

#endif//OUTPUT_MEDIA_PIN_H
