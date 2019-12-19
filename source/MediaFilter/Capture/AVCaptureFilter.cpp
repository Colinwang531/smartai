#include "boost/winapi/dll.hpp"
#include "error.h"
#include "Url/Url.h"
using URL = framework::wrapper::URL;
#include "MediaFilter/Capture/AVCaptureFilter.h"

namespace framework
{
	namespace multimedia
	{
		AVCaptureFilter::AVCaptureFilter()
			: SourceMediaFilter(), dllOpenStreamFunc{ NULL }, dllCloseStreamFunc{ NULL }, streamID{ 0 }
		{}

		AVCaptureFilter::~AVCaptureFilter()
		{}

		int AVCaptureFilter::openStream(const std::string url)
		{
			int status{ url.empty() ? ERR_INVALID_PARAM : ERR_OK };

			if (ERR_OK == status)
			{
				std::string dllFileName, openStreamFuncName, closeStreamFuncName;
				URL streamUrl{ url };
				framework::wrapper::URLProtocol protocol{ streamUrl.getProtocol() };

				if (framework::wrapper::URLProtocol::URL_PROTOCOL_LIVESTREAM == protocol)
				{
					dllFileName.append(".\\MediaStreamClient.dll");
					openStreamFuncName.append("MEDIASTREAMCLIENT_OpenStream");
					closeStreamFuncName.append("MEDIASTREAMCLIENT_CloseStream");
				}
				else if (framework::wrapper::URLProtocol::URL_PROTOCOL_PLAYBACK == protocol)
				{
				}
				else if (framework::wrapper::URLProtocol::URL_PROTOCOL_RTSP == protocol)
				{
				}
				else
				{
					status = ERR_NOT_SUPPORT;
				}

				if (!dllFileName.empty() && ERR_OK == status)
				{
					boost::winapi::HMODULE_ hmodule{ LoadLibraryA(dllFileName.c_str()) };
					if (hmodule)
					{
						dllOpenStreamFunc = (DllOpenStreamFunc)GetProcAddress(hmodule, openStreamFuncName.c_str());
						dllCloseStreamFunc = (DllCloseStreamFunc)GetProcAddress(hmodule, closeStreamFuncName.c_str());

						if (dllOpenStreamFunc)
						{
							streamID = dllOpenStreamFunc(url.c_str(), &AVCaptureFilter::captureMediaStreamDataCallback, this);
							status = 0 < streamID ? ERR_OK : ERR_BAD_OPERATE;
						}

						FreeLibrary(hmodule);
					}
				}
			}

			return status;
		}

		int AVCaptureFilter::closeStream(void)
		{
			int status{ 0 < streamID ? ERR_OK : ERR_BAD_OPERATE };

			if (ERR_OK == status && dllCloseStreamFunc)
			{
				status = dllCloseStreamFunc(streamID);
			}

			return status;
		}

		void AVCaptureFilter::captureMediaStreamDataCallback(
			const int streamID /* = -1 */, const unsigned char event /* = 0 */, 
			const unsigned char* data /* = NULL */, const unsigned long long dataBytes /* = 0 */, void* userData /* = NULL */)
		{
			AVCaptureFilter* filter{ reinterpret_cast<AVCaptureFilter*>(userData) };

			if (filter && filter->streamID == streamID)
			{
				enum
				{
					TCP_CLIENT_EVENT_NONE = 0,
					TCP_CLIENT_EVENT_CONNECTED,
					TCP_CLIENT_EVENT_DISCONNECTED,
					TCP_CLIENT_EVENT_READ
				};

				if (TCP_CLIENT_EVENT_CONNECTED == event)
				{
				}
				else if (TCP_CLIENT_EVENT_DISCONNECTED == event)
				{
				}
				else if (TCP_CLIENT_EVENT_READ == event)
				{
				}
			}
		}
	}//namespace multimedia
}//namespace framework
