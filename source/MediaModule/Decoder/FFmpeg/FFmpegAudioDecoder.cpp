#include "error.h"
#include "MediaModel/Decoder/FFmpeg/FFmpegAudioDecoder.h"

namespace framework
{
	namespace multimedia
	{
		FFmpegAudioDecoder::FFmpegAudioDecoder() : MediaDecoder()
		{}

		FFmpegAudioDecoder::~FFmpegAudioDecoder(void)
		{}

		int FFmpegAudioDecoder::inputMediaData(MediaDataPtr mediaData)
		{
			return ERR_NOT_SUPPORT;
		}
	}//namespace multimedia
}//namespace framework
