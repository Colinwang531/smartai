#include "boost/bind.hpp"
#include "boost/make_shared.hpp"
#include "error.h"
#include "MediaModel/Capture/Hikvision/HikvisionSDKCapture.h"
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
				const MediaDataMainID mediaDataMainID{ mediaData->getMainID() };

				if (MediaDataMainID::MEDIA_DATA_MAIN_ID_FILE == mediaDataMainID)
				{
					MediaModelPtr ffmpegLocalFileDemuxerPtr{ boost::make_shared<FFmpegLocalFileDemuxer>() };
					if (ffmpegLocalFileDemuxerPtr)
					{
						mediaModelPtr.swap(ffmpegLocalFileDemuxerPtr);
					}
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
