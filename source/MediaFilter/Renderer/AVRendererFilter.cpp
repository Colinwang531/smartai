#include "boost/make_shared.hpp"
#include "error.h"
#include "MediaData/MediaData.h"
#include "MediaModel/Renderer/D3D/VideoD3DRenderer.h"
// #include "MediaModel/Renderer/DXS/AudioDxSoundPlayer.h"
// using AudioDxSoundPlayer = NS(model, 1)::AudioDxSoundPlayer;
#include "MediaPin/MediaPin.h"
#include "MediaFilter/Renderer/AVRendererFilter.h"

namespace framework
{
	namespace multimedia
	{
		AVRendererFilter::AVRendererFilter() : MediaFilter(), videoRendererWnd{ NULL }
		{}

		AVRendererFilter::~AVRendererFilter()
		{}

		int AVRendererFilter::createNewFilter(const std::string& streamURL)
		{
			int status{ streamURL.empty() ? ERR_INVALID_PARAM : ERR_OK };

			if (ERR_OK == status)
			{
				std::string inputPinID, outputPinID;
				if (!streamURL.compare("video"))
				{
					inputPinID = VideoStreamInputPinID;
					outputPinID = VideoStreamOutputPinID;
				}
				else if (!streamURL.compare("audio"))
				{
					inputPinID = AudioStreamInputPinID;
					outputPinID = AudioStreamOutputPinID;
				}

				if (!inputPinID.empty() && !outputPinID.empty() &&
					ERR_OK == MediaFilter::createNewInputPin(inputPinID) &&
					ERR_OK == MediaFilter::createNewOutputPin(outputPinID))
				{
					status = ERR_OK;
				}
			}

			return status;
		}

		int AVRendererFilter::createNewModel(MediaDataPtr mediaData)
		{
			int status{ mediaData ? ERR_OK : ERR_INVALID_PARAM };

			if (ERR_OK == status)
			{
				const MediaDataMainID mediaDataMainID{ mediaData->getMainID() };
				const MediaDataSubID mediaDataSubID{ mediaData->getSubID() };

				if (MediaDataMainID::MEDIA_DATA_MAIN_ID_VIDEO == mediaDataMainID)
				{
					MediaModelPtr dxVideoD3dRendererPtr{ 
						boost::make_shared<VideoD3DRenderer>(reinterpret_cast<HWND>(videoRendererWnd)) };
					if (dxVideoD3dRendererPtr)
					{
						mediaModelPtr.swap(dxVideoD3dRendererPtr);
					}
				}
				if (MediaDataMainID::MEDIA_DATA_MAIN_ID_AUDIO == mediaDataMainID)
				{
					// 				MediaModelPtr dxSoundPlayerPtr{ boost::make_shared<AudioDxSoundPlayer>() };
// 				if (dxSoundPlayerPtr)
// 				{
// 					mediaModelPtr.swap(dxSoundPlayerPtr);
// 				}
				}
			}

			return ERR_OK == status ? MediaFilter::createNewModel(mediaData) : status;
		}
	}//namespace multimedia
}//namespace framework
