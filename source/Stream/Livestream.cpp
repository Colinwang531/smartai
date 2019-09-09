#include "error.h"
#include "Stream/Livestream.h"

NS_BEGIN(stream, 1)

Livestream::Livestream() : AVStream(), livestreamID{ -1 }
{}

Livestream::~Livestream()
{}

int Livestream::open(const int userID /* = -1 */, const int streamNo /* = -1 */)
{
	int status{ ERR_INVALID_PARAM };

	if (-1 < streamNo && -1 < userID)
	{
		streamNumber = streamNo;
		status = ERR_OK;
	}

	return status;
}

int Livestream::close()
{
	return -1 < livestreamID ? ERR_OK : ERR_BAD_OPERATE;
}

int Livestream::capture(
	const int userID, const int cameraIndex, char*& jpegData, const int jpegBytes /* = 1024 * 1024 */)
{
	return -1 < livestreamID && -1 < userID && -1 < cameraIndex && jpegData && 0 < jpegBytes ? ERR_OK : ERR_BAD_OPERATE;
}

NS_END
