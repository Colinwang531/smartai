#include "error.h"
#include "MediaFilter/SourceMediaFilter.h"

NS_BEGIN(filter, 1)

SourceMediaFilter::SourceMediaFilter() : MediaFilter()
{}

SourceMediaFilter::~SourceMediaFilter()
{}

bool SourceMediaFilter::isSourceFilter() const
{
	return true;
}

NS_END
