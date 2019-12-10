#include "error.h"
#include "MediaFilter/SourceMediaFilter.h"

namespace framework
{
	namespace multimedia
	{
		SourceMediaFilter::SourceMediaFilter() : MediaFilter()
		{}

		SourceMediaFilter::~SourceMediaFilter()
		{}

		int SourceMediaFilter::createNewModule(MediaDataPtr mediaData)
		{
			return ERR_NOT_SUPPORT;
		}
	}//namespace multimedia
}//namespace framework
