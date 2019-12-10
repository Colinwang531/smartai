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
			const framework::wrapper::URLProtocol protocol{ URL(streamURL).getProtocol() };

			if (framework::wrapper::URLProtocol::URL_PROTOCOL_LIVESTREAM == protocol)
			{
				const std::string streamValue{ URL(streamURL).getParameter("stream") };
				if (!streamValue.compare("hikvision"))
				{
					MediaModulePtr mediaModulePtr{ boost::make_shared<HikvisionSDKCapture>() };
					if (mediaModulePtr)
					{
						this->mediaModulePtr.swap(mediaModulePtr);
					}
				}
				else if (!streamValue.compare("dahua"))
				{

				}
			}
			else if (framework::wrapper::URLProtocol::URL_PROTOCOL_PLAYBACK == protocol)
			{
				const std::string filePath{ URL(streamURL).getParameter("path") };
				if (!filePath.empty())
				{
					MediaModulePtr mediaModulePtr{ boost::make_shared<FFmpegLocalFileDemuxer>() };
					if (mediaModulePtr)
					{
						this->mediaModulePtr.swap(mediaModulePtr);
					}
				}
			}

			status = MediaFilter::createNewModule(NULL);
			boost::shared_ptr<MediaCapture> mediaCapturePtr{ 
				boost::dynamic_pointer_cast<MediaCapture>(mediaModulePtr) };
			if (mediaCapturePtr)
			{
				status = mediaCapturePtr->openStream(streamURL);
			}

			return status;
		}

		int AVCaptureFilter::createNewFilter()
		{
			return ERR_OK == MediaFilter::createNewOutputPin(VideoStreamOutputPinID) &&
				ERR_OK == MediaFilter::createNewOutputPin(AudioStreamOutputPinID) ? ERR_OK : ERR_BAD_ALLOC;
		}
	}//namespace multimedia
}//namespace framework
