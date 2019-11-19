#include "error.h"
#include "MediaModel/Accessor/MediaAccessor.h"

NS_BEGIN(model, 1)

MediaAccessor::MediaAccessor() : MediaModel()
{}

MediaAccessor::~MediaAccessor()
{}

int MediaAccessor::openStream(
	const std::string name, const std::string password, 
	const std::string address, const int port /* = 8000 */, 
	const int channel /* = 0 */)
{
	return !name.empty() && !password.empty() && 
		!address.empty() && 0 < port && 0 <= channel ? ERR_OK : ERR_INVALID_PARAM;
}

int MediaAccessor::closeStream(void)
{
	return ERR_OK;
}

NS_END
