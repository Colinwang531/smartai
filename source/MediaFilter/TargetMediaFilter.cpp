#include "error.h"
#include "MediaFilter/TargetMediaFilter.h"

namespace framework
{
	namespace multimedia
	{
		TargetMediaFilter::TargetMediaFilter() : MediaFilter(), userID{ -1 }
		{}

		TargetMediaFilter::~TargetMediaFilter()
		{}

// 		int TargetMediaFilter::inputMediaData(MediaDataPtr mediaData)
// 		{
// 			if (mediaData && callbackMediaDataCallback)
// 			{
// 				callbackMediaDataCallback(userID, mediaData);
// 			}
// 
// 			return ERR_OK;
// 		}
	}//namespace multimedia
}//namespace framework
