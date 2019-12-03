#include "boost/algorithm/string.hpp"
#include "error.h"
#include "MediaData/MediaData.h"
#include "MediaModel/Capture/MediaCapture.h"

namespace framework
{
	namespace multimedia
	{
		MediaCapture::MediaCapture() : MediaModel()
		{}

		MediaCapture::~MediaCapture()
		{
			closeStream();
		}

		int MediaCapture::inputMediaData(MediaDataPtr mediaData)
		{
			int status{ 
				mediaData && MediaDataMainID::MEDIA_DATA_MAIN_ID_STREAM == mediaData->getMainID() ? ERR_OK : ERR_INVALID_PARAM };

			if (ERR_OK == status)
			{
				const char* streamUrl{ (const char*)(mediaData->getData()) };
				void* hwnd{ mediaData->getUserData() };

				std::vector<std::string> streamUrlSegment;
				boost::split(streamUrlSegment, streamUrl, boost::is_any_of(":"));
				const int port{ atoi(streamUrlSegment[3].c_str()) }, channel{ atoi(streamUrlSegment[4].c_str()) };
				status = openStream(streamUrlSegment[0], streamUrlSegment[1], streamUrlSegment[2], port, channel, hwnd);
			}

			return status;
		}
	}
}
