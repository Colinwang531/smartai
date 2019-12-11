#include "boost/make_shared.hpp"
#include "error.h"
#include "URL/Url.h"
using URL = framework::wrapper::URL;
#include "MediaModule/Capture/Hikvision/HikvisionSDKCapture.h"
#include "MediaModule/Capture/FFmpeg/FFmpegLocalFileDemuxer.h"
#include "MediaPin/MediaPin.h"
#include "MediaFilter/Capture/AVCaptureFilter.h"

namespace framework
{
	namespace multimedia
	{
		AVCaptureFilter::AVCaptureFilter() : SourceMediaFilter()
		{}

		AVCaptureFilter::~AVCaptureFilter()
		{}

		int AVCaptureFilter::startCapture(const std::string& streamURL)
		{
			int status{ ERR_NOT_SUPPORT };
			MediaModulePtr mediaModulePtr;
			const framework::wrapper::URLProtocol protocol{ URL(streamURL).getProtocol() };

			if (framework::wrapper::URLProtocol::URL_PROTOCOL_LIVESTREAM == protocol)
			{
				const DeviceType streamValue{ (const DeviceType)atoi(URL(streamURL).getParameter("stream").c_str()) };
				if (DeviceType::DEVICE_TYPE_HIKVISION == streamValue)
				{
					mediaModulePtr = boost::make_shared<HikvisionSDKCapture>();
					if (mediaModulePtr)
					{
						this->mediaModulePtr.swap(mediaModulePtr);
					}
				}
				else if (DeviceType::DEVICE_TYPE_DAHUA == streamValue)
				{

				}
			}
			else if (framework::wrapper::URLProtocol::URL_PROTOCOL_PLAYBACK == protocol)
			{
				const std::string filePath{ URL(streamURL).getParameter("path") };
				if (!filePath.empty())
				{
					mediaModulePtr = boost::make_shared<FFmpegLocalFileDemuxer>();
					if (mediaModulePtr)
					{
						this->mediaModulePtr.swap(mediaModulePtr);
					}
				}
			}

			if (this->mediaModulePtr)
			{
				MediaFilter::setPostInputMediaDataCallback();
				status = boost::dynamic_pointer_cast<MediaCapture>(this->mediaModulePtr)->startCapture(streamURL);
			}

			return status;
		}

		int AVCaptureFilter::stopCapture(void)
		{
			return ERR_OK;
		}

		int AVCaptureFilter::createNewFilter()
		{
			return ERR_OK == MediaFilter::createNewOutputPin(VideoStreamOutputPinID) &&
				ERR_OK == MediaFilter::createNewOutputPin(AudioStreamOutputPinID) ? ERR_OK : ERR_BAD_ALLOC;
		}
	}//namespace multimedia
}//namespace framework
