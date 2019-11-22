#include "MediaFilter/TargetMediaFilter.h"

NS_BEGIN(filter, 1)

TargetMediaFilter::TargetMediaFilter() 
	: MediaFilter(), postMediaDataCallback{ NULL }, cbUserData{ NULL }, mediaStreamID{ 0 }
{}

TargetMediaFilter::~TargetMediaFilter()
{}

NS_END
