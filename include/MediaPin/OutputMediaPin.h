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
			int inputMediaData(MediaData* mediaData = NULL) override;

		protected:
			int connectPin(const MediaPin* pin = NULL) override;

		private:
			Vector<MediaPin*> inputPinRefGroup;
		};//class OutputMediaPin
	}//namespace multimedia
}//namespace framework

#endif//OUTPUT_MEDIA_PIN_H
