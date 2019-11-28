#include "boost/make_shared.hpp"
#include "error.h"
#include "MediaPin/MediaPin.h"
// #include "MediaModel/Renderer/D3D/VideoD3DRenderer.h"
// using VideoD3DRenderer = NS(model, 1)::VideoD3DRenderer;
// #include "MediaModel/Renderer/DXS/AudioDxSoundPlayer.h"
// using AudioDxSoundPlayer = NS(model, 1)::AudioDxSoundPlayer;
#include "MediaFilter/Renderer/AVRendererFilter.h"

namespace framework
{
	namespace multimedia
	{

		AVRendererFilter::AVRendererFilter() : MediaFilter()
		{}

		AVRendererFilter::~AVRendererFilter()
		{}

		int AVRendererFilter::createNewFilter(
			const MediaStreamID mediaStreamID /* = MediaStreamID::MEDIA_STREAM_ID_AV */)
		{
			if (MediaStreamID::MEDIA_STREAM_ID_AV == mediaStreamID || MediaStreamID::MEDIA_STREAM_ID_VIDEO == mediaStreamID)
			{
				createNewInputPin(VideoStreamInputPinID);
				createNewOutputPin(VideoStreamOutputPinID);
// 				MediaModelPtr dxVideoD3dRendererPtr{ boost::make_shared<VideoD3DRenderer>() };
// 				if (dxVideoD3dRendererPtr)
// 				{
// 					mediaModelPtr.swap(dxVideoD3dRendererPtr);
// 				}
			}
			if (MediaStreamID::MEDIA_STREAM_ID_AV == mediaStreamID || MediaStreamID::MEDIA_STREAM_ID_AUDIO == mediaStreamID)
			{
				createNewInputPin(AudioStreamInputPinID);
				createNewOutputPin(AudioStreamOutputPinID);
// 				MediaModelPtr dxSoundPlayerPtr{ boost::make_shared<AudioDxSoundPlayer>() };
// 				if (dxSoundPlayerPtr)
// 				{
// 					mediaModelPtr.swap(dxSoundPlayerPtr);
// 				}
			}

			return MediaFilter::createNewFilter(mediaStreamID);
		}

		int AVRendererFilter::inputMediaData(MediaDataPtr mediaData)
		{
			return ERR_OK;
// 			if (MediaDataSubID::MEDIA_DATA_SUB_ID_NV12 == mediaData->getSubID())
// 			{
// 				return MediaFilter::postInputMediaData(mediaData);
// 			}
// 
// 			return mediaData && mediaModelPtr ? mediaModelPtr->inputMediaData(mediaData) : ERR_INVALID_PARAM;
		}

	}//namespace multimedia
}//namespace framework
