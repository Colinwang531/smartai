#include "error.h"
#include "MediaFilter/TargetMediaFilter.h"

namespace framework
{
	namespace multimedia
	{
		TargetMediaFilter::TargetMediaFilter()
			: MediaFilter()//, postMediaDataCallback{ NULL }, cbUserData{ NULL }, mediaStreamID{ 0 }
		{}

		TargetMediaFilter::~TargetMediaFilter()
		{}

		int TargetMediaFilter::inputMediaData(MediaDataPtr mediaData)
		{
			int status{ mediaData ? ERR_OK : ERR_INVALID_PARAM };
			return status;
		}
	}//namespace multimedia
}//namespace framework
