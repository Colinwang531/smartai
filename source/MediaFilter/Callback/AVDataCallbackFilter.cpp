#include "boost/make_shared.hpp"
#include "error.h"
#include "MediaPin/MediaPin.h"
#include "MediaFilter/Callback/AVDataCallbackFilter.h"

namespace framework
{
	namespace multimedia
	{
		AVDataCallbackFilter::AVDataCallbackFilter() : TargetMediaFilter()
		{}

		AVDataCallbackFilter::~AVDataCallbackFilter()
		{}

		int AVDataCallbackFilter::createNewFilter(const MediaStreamID mediaStreamID)
		{
			if (MediaStreamID::MEDIA_STREAM_ID_AV == mediaStreamID || MediaStreamID::MEDIA_STREAM_ID_VIDEO == mediaStreamID)
			{
				createNewInputPin(VideoStreamInputPinID);
			}
			if (MediaStreamID::MEDIA_STREAM_ID_AV == mediaStreamID || MediaStreamID::MEDIA_STREAM_ID_AUDIO == mediaStreamID)
			{
				createNewInputPin(AudioStreamInputPinID);
			}

			return MediaFilter::createNewFilter(mediaStreamID);
		}

		int AVDataCallbackFilter::inputMediaData(MediaDataPtr mediaData)
		{
			return ERR_OK;
		}
	}//namespace multimedia
}//namespace framework
