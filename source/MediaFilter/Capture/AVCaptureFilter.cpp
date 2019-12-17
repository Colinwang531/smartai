#include "boost/winapi/dll.hpp"
#include "error.h"
#include "MediaFilter/Capture/AVCaptureFilter.h"

namespace framework
{
	namespace multimedia
	{
		AVCaptureFilter::AVCaptureFilter() : SourceMediaFilter(), openStreamFunc{ NULL }, closeStreamFunc{ NULL }
		{}

		AVCaptureFilter::~AVCaptureFilter()
		{}

		int AVCaptureFilter::openStream(const std::string url, void* hwnd /* = NULL */)
		{
			int status{ ERR_NOT_SUPPORT };

			boost::winapi::HMODULE_ hmodule{ LoadLibraryA(".\\AVCapture.dll") };
			if (hmodule)
			{
				openStreamFunc = (OpenStreamFunc)GetProcAddress(hmodule, "AVCAPTURE_OpenStream");
				closeStreamFunc = (CloseStreamFunc)GetProcAddress(hmodule, "AVCAPTURE_CloseStream");

				if (openStreamFunc && closeStreamFunc)
				{
					status = openStreamFunc(url.c_str(), hwnd, &AVCaptureFilter::captureFrameCallback, this);
				}

				FreeLibrary(hmodule);
			}

			return status;
		}

		int AVCaptureFilter::closeStream(void)
		{
			int status{ ERR_BAD_OPERATE };

			if (closeStreamFunc)
			{
				status = closeStreamFunc();
			}

			return status;
		}

		void AVCaptureFilter::captureFrameCallback(
			const int playID /* = -1 */, const unsigned char frameType /* = 0 */, 
			const unsigned char* frameData /* = NULL */, const int frameBytes /* = 0 */, void* userData /* = NULL */)
		{
		}
	}//namespace multimedia
}//namespace framework
