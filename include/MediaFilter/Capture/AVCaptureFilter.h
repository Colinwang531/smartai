// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Stream data capture.
//

#ifndef AV_CAPTURE_FILTER_H
#define AV_CAPTURE_FILTER_H

#include "MediaFilter/SourceMediaFilter.h"

namespace framework
{
	namespace multimedia
	{
		typedef enum class tagDeviceType_t : unsigned char
		{
			DEVICE_TYPE_NONE = 0,
			DEVICE_TYPE_HIKVISION,
			DEVICE_TYPE_DAHUA
		}DeviceType;

		class AVCaptureFilter : public SourceMediaFilter
		{
		public:
			AVCaptureFilter(void);
			virtual ~AVCaptureFilter(void);

		public:
			int startCapture(const std::string& streamURL);
			int stopCapture(void);

		protected:
			int createNewFilter(void) override;
		};//class AVCaptureFilter
	}//namespace multimedia
}//namespace framework

#endif//AV_CAPTURE_FILTER_H
