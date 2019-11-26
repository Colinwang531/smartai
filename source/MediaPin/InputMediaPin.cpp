#include "error.h"
#include "MediaData/MediaData.h"
#include "MediaFilter/MediaFilter.h"
#include "MediaPin/InputMediaPin.h"

namespace framework
{
	namespace multimedia
	{
		InputMediaPin::InputMediaPin(const MediaFilter& filter) : MediaPin(), mediaFilter{ filter }
		{}

		InputMediaPin::~InputMediaPin()
		{}

		int InputMediaPin::inputMediaData(MediaData* mediaData /* = NULL */)
		{
			return mediaData ? mediaFilter.inputMediaData(mediaData) : ERR_INVALID_PARAM;
		}
	}//namespace multimedia
}//namespace framework
