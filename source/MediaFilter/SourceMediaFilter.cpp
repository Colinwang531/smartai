#include "error.h"
#include "MediaFilter/SourceMediaFilter.h"

NS_BEGIN(filter, 1)

SourceMediaFilter::SourceMediaFilter() : MediaFilter(MediaFilterMode::MEDIA_FILTER_MODE_SOURCE)
{}

SourceMediaFilter::~SourceMediaFilter()
{}

int SourceMediaFilter::inputMediaData(MediaDataPtr mediaData)
{
	return ERR_NOT_SUPPORT;
}

NS_END
