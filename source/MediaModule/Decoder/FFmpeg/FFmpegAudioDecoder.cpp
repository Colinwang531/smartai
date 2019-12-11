#include "error.h"
#include "MediaModule/Decoder/FFmpeg/FFmpegAudioDecoder.h"

namespace framework
{
	namespace multimedia
	{
		FFmpegAudioDecoder::FFmpegAudioDecoder() : MediaModule()
		{}

		FFmpegAudioDecoder::~FFmpegAudioDecoder(void)
		{}

		int FFmpegAudioDecoder::inputMediaData(MediaDataPtr mediaData)
		{
			return ERR_NOT_SUPPORT;
		}
	}//namespace multimedia
}//namespace framework
