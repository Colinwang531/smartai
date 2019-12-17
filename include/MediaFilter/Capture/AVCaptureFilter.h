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
		typedef void (CALLBACK* CaptureFrameCallback)(const int, const unsigned char, const unsigned char*, const int, void*);
		typedef int (CALLBACK* OpenStreamFunc)(const char*, void*, CaptureFrameCallback, void*);
		typedef int (CALLBACK* CloseStreamFunc)(void);

		class AVCaptureFilter : public SourceMediaFilter
		{
		public:
			AVCaptureFilter(void);
			virtual ~AVCaptureFilter(void);

		public:
			int openStream(const std::string url, void* hwnd = NULL);
			int closeStream(void);

		private:
			static void captureFrameCallback(
				const int playID = -1, const unsigned char frameType = 0, 
				const unsigned char* frameData = NULL, const int frameBytes = 0, void* userData = NULL);

		private:
			OpenStreamFunc openStreamFunc;
			CloseStreamFunc closeStreamFunc;
		};//class AVCaptureFilter
	}//namespace multimedia
}//namespace framework

#endif//AV_CAPTURE_FILTER_H
