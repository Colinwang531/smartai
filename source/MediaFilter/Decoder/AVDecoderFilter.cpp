#include "boost/make_shared.hpp"
#include "error.h"
#include "MediaPin/MediaPin.h"
#include "MediaModel/Decoder/FFmpeg/FFmpegVideoDecoder.h"
#include "MediaModel/Decoder/FFmpeg/FFmpegAudioDecoder.h"
#include "MediaFilter/Decoder/AVDecoderFilter.h"

namespace framework
{
	namespace multimedia
	{
		AVDecoderFilter::AVDecoderFilter() : MediaFilter()
		{}

		AVDecoderFilter::~AVDecoderFilter()
		{}

		int AVDecoderFilter::createNewFilter(
			const MediaStreamID mediaStreamID /* = MediaStreamID::MEDIA_STREAM_ID_AV */)
		{
			if (MediaStreamID::MEDIA_STREAM_ID_AV == mediaStreamID || MediaStreamID::MEDIA_STREAM_ID_VIDEO == mediaStreamID)
			{
				createNewInputPin(VideoStreamInputPinID);
				createNewOutputPin(VideoStreamOutputPinID);
				MediaModelPtr videoDecoderPtr{ boost::make_shared<FFmpegVideoDecoder>() };
				if (videoDecoderPtr)
				{
					mediaModelPtr.swap(videoDecoderPtr);
				}
			}
			if (MediaStreamID::MEDIA_STREAM_ID_AV == mediaStreamID || MediaStreamID::MEDIA_STREAM_ID_AUDIO == mediaStreamID)
			{
				createNewInputPin(AudioStreamInputPinID);
				createNewOutputPin(AudioStreamOutputPinID);
				MediaModelPtr audioDecoderPtr{ boost::make_shared<FFmpegAudioDecoder>() };
				if (audioDecoderPtr)
				{
					mediaModelPtr.swap(audioDecoderPtr);
				}
			}

			return MediaFilter::createNewFilter(mediaStreamID);
		}

		int AVDecoderFilter::inputMediaData(MediaDataPtr mediaData)
		{
// 			if (MediaDataSubID::MEDIA_DATA_SUB_ID_HIKVISION == mediaData->getSubID() && !resetModel)
// 			{
// 				MediaModelPtr hikvisionSdkDecoderPtr{ boost::make_shared<HikvisionSDKDecoder>() };
// 				if (hikvisionSdkDecoderPtr)
// 				{
// 					hikvisionSdkDecoderPtr->setPostInputMediaDataCallback(
// 						boost::bind(&AVDecoderFilter::postInputMediaData, this, _1));
// 					mediaModelPtr.swap(hikvisionSdkDecoderPtr);
// 					resetModel = true;
// 				}
// 			}

			return mediaData && mediaModelPtr ? mediaModelPtr->inputMediaData(mediaData) : ERR_INVALID_PARAM;
		}
	}//namespace multimedia
}//namespace framework
