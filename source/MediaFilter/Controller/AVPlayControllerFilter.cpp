#include "boost/make_shared.hpp"
#include "error.h"
#include "MediaPin/MediaPin.h"
#include "MediaFilter/Controller/AVPlayControllerFilter.h"

namespace framework
{
	namespace multimedia
	{
		AVPlayControllerFilter::AVPlayControllerFilter() : MediaFilter()
		{}

		AVPlayControllerFilter::~AVPlayControllerFilter()
		{}

		int AVPlayControllerFilter::createNewFilter(
			const MediaStreamID mediaStreamID /* = MediaStreamID::MEDIA_STREAM_ID_AV */)
		{
			if (MediaStreamID::MEDIA_STREAM_ID_AV == mediaStreamID || MediaStreamID::MEDIA_STREAM_ID_VIDEO == mediaStreamID)
			{
				createNewInputPin(VideoStreamInputPinID);
				createNewOutputPin(VideoStreamOutputPinID);
			}
			if (MediaStreamID::MEDIA_STREAM_ID_AV == mediaStreamID || MediaStreamID::MEDIA_STREAM_ID_AUDIO == mediaStreamID)
			{
				createNewInputPin(AudioStreamInputPinID);
				createNewOutputPin(AudioStreamOutputPinID);
			}

			return MediaFilter::createNewFilter(mediaStreamID);
		}

		int AVPlayControllerFilter::inputMediaData(MediaDataPtr mediaData)
		{
			return MediaFilter::inputMediaData(mediaData);
		}

		int AVPlayControllerFilter::startPlay()
		{
			return ERR_OK;
		}

		int AVPlayControllerFilter::stopPlay()
		{
			return ERR_OK;
		}

		int AVPlayControllerFilter::pausePlay()
		{
			return ERR_OK;
		}

		int AVPlayControllerFilter::fastPlay(const short speed /* = 1 */)
		{
			return checkSpeedValue(speed);
		}

		int AVPlayControllerFilter::slowPlay(const short speed /* = -1 */)
		{
			return checkSpeedValue(speed);
		}

		int AVPlayControllerFilter::checkSpeedValue(const short speed /* = 1 */)
		{
			return 1 == speed || 2 == speed || 4 == speed || 8 == speed || 16 == speed ||
				-1 == speed || -2 == speed || -4 == speed || -8 == speed || -16 == speed ?
				ERR_OK : ERR_INVALID_PARAM;
		}
	}//namespace multimedia
}//namespace framework
