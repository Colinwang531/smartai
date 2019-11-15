#include "error.h"
#include "MediaFilter/SourceMediaFilter.h"

NS_BEGIN(filter, 1)

SourceMediaFilter::SourceMediaFilter() : MediaFilter()
{}

SourceMediaFilter::~SourceMediaFilter()
{}

int SourceMediaFilter::inputMediaData(MediaDataPtr mediaData)
{
	return ERR_NOT_IMPLEMENT;
}

NS_END
