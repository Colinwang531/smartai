#include "error.h"
#include "MediaData/MediaData.h"
#include "MediaFilter/MediaFilter.h"
#include "MediaPin/InputMediaPin.h"

namespace framework
{
	namespace multimedia
	{
		InputMediaPin::InputMediaPin(MediaFilterRef filter)
			: MediaPin(), mediaFilterRef{ filter }
		{}

		InputMediaPin::~InputMediaPin()
		{}

		int InputMediaPin::inputMediaData(MediaDataPtr mediaData)
		{
			int status{ mediaData ? ERR_OK : ERR_INVALID_PARAM };

			if (ERR_OK == status && !mediaFilterRef.expired())
			{
				status = mediaFilterRef.lock()->inputMediaData(mediaData);
			}

			return status;
		}
	}//namespace multimedia
}//namespace framework
