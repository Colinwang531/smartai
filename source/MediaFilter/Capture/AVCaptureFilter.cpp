#include "boost/make_shared.hpp"
#include "error.h"
#include "URL/Url.h"
using URL = framework::wrapper::URL;
#include "MediaData/MediaData.h"
#include "MediaModel/Capture/Hikvision/HikvisionSDKCapture.h"
#include "MediaModel/Demuxer/FFmpeg/FFmpegLocalFileDemuxer.h"
#include "MediaFilter/Capture/AVCaptureFilter.h"

namespace framework
{
	namespace multimedia
	{
		AVCaptureFilter::AVCaptureFilter() : SourceMediaFilter()
		{}

		AVCaptureFilter::~AVCaptureFilter()
		{}

		int AVCaptureFilter::createNewModel(MediaDataPtr mediaData)
		{
			int status{ mediaData ? ERR_OK : ERR_INVALID_PARAM };
			
			if (ERR_OK == status)
			{
				URL url;
				url.setUrl((const char*)mediaData->getData());
				const framework::wrapper::URLProtocol protocol{ url.getProtocol() };

				if (framework::wrapper::URLProtocol::URL_PROTOCOL_LIVESTREAM == protocol)
				{
					const std::string stream{ url.getParameter("stream") };
					if (!stream.compare("hikvision"))
					{
						MediaModelPtr hikvisionSDKCapturePtr{ boost::make_shared<HikvisionSDKCapture>() };
						if (hikvisionSDKCapturePtr)
						{
							mediaModelPtr.swap(hikvisionSDKCapturePtr);
						}
					}
				} if (framework::wrapper::URLProtocol::URL_PROTOCOL_PLAYBACK == protocol)
				{
					const std::string stream{ url.getParameter("path") };
					if (!stream.empty())
					{
						MediaModelPtr ffmpegLocalFileDemuxerPtr{ boost::make_shared<FFmpegLocalFileDemuxer>() };
						if (ffmpegLocalFileDemuxerPtr)
						{
							mediaModelPtr.swap(ffmpegLocalFileDemuxerPtr);
						}
					}
				}
				else if (framework::wrapper::URLProtocol::URL_PROTOCOL_RTSP == protocol)
				{
				}
				else
				{
					status = ERR_NOT_SUPPORT;
				}
			}

			return ERR_OK == status ? MediaFilter::createNewModel(mediaData) : status;
		}
	}//namespace multimedia
}//namespace framework
