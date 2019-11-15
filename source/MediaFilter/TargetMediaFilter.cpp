#include "error.h"
#include "MediaFilter/TargetMediaFilter.h"

NS_BEGIN(filter, 1)

TargetMediaFilter::TargetMediaFilter() : MediaFilter()
{}

TargetMediaFilter::~TargetMediaFilter()
{}

int TargetMediaFilter::postMediaDataCallback(MediaDataPtr mediaData)
{
	return ERR_NOT_IMPLEMENT;
}

NS_END
