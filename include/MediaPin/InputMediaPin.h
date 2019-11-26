// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Base class of pin for inputing.
//

#ifndef INPUT_MEDIA_PIN_H
#define INPUT_MEDIA_PIN_H

#include "MediaPin.h"

namespace framework
{
	namespace multimedia
	{
		class MediaFilter;

		class InputMediaPin : public MediaPin
		{
		public:
			InputMediaPin(const MediaFilter& filter);
			virtual ~InputMediaPin(void);

		public:
			int inputMediaData(MediaData* mediaData = NULL) override;

		protected:
			int connectPin(const MediaPin* pin = NULL) override
			{
				return ERR_NOT_SUPPORT;
			}
			bool isInputPin(void) const override
			{
				return true;
			}

		private:
			const MediaFilter& mediaFilter;
		};//class InputMediaPin
	}//namespace multimedia
}//namespace framework

#endif//INPUT_MEDIA_PIN_H
