#include "MediaFilter/TargetMediaFilter.h"

NS_BEGIN(filter, 1)

TargetMediaFilter::TargetMediaFilter() : MediaFilter()
{}

TargetMediaFilter::~TargetMediaFilter()
{}

bool TargetMediaFilter::isTargetFilter() const
{
	return false;
}

NS_END
