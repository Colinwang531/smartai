// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Stream capture.
//

#ifndef AV_CAPTURE_FILTER_H
#define AV_CAPTURE_FILTER_H

#include <windows.h>
#include "MediaFilter/SourceMediaFilter.h"

namespace framework
{
	namespace multimedia
	{
		typedef void (CALLBACK* MediaStreamDataCallback)(
			const int, const unsigned char, const unsigned char*, const unsigned long long, void*);
		typedef int (CALLBACK* DllOpenStreamFunc)(const char*, MediaStreamDataCallback, void*);
		typedef int (CALLBACK* DllCloseStreamFunc)(const int);

		class AVCaptureFilter : public SourceMediaFilter
		{
		public:
			AVCaptureFilter(void);
			virtual ~AVCaptureFilter(void);

		public:
			int openStream(const std::string url);
			int closeStream(void);

		private:
			static void captureMediaStreamDataCallback(
				const int streamID = -1, const unsigned char event = 0, 
				const unsigned char* data = NULL, const unsigned long long dataBytes = 0, void* userData = NULL);

		private:
			DllOpenStreamFunc dllOpenStreamFunc;
			DllCloseStreamFunc dllCloseStreamFunc;
			int streamID;
		};//class AVCaptureFilter
	}//namespace multimedia
}//namespace framework

#endif//AV_CAPTURE_FILTER_H
