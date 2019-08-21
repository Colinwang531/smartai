#include "error.h"
#include "BGR24FrameCache.h"

BGR24FrameCache::BGR24FrameCache(const int frames /* = 5 */) : maxFrameNumber{ frames }
{}

BGR24FrameCache::~BGR24FrameCache()
{}

int BGR24FrameCache::insert(
	const int channel /* = -1 */, const char* ip /* = NULL */, 
	const int ipBytes /* = 0 */, const char* frame /* = NULL */, 
	const int frameBytes /* = 0 */, const char* jpeg /* = NULL */, 
	const int jpegBytes /* = 0 */)
{
	int status{ ERR_INVALID_PARAM };

	if (frame && 0 < frameBytes)
	{
		rwLock.lock();
		if (maxFrameNumber >= BGR24FrameVect.size())
		{
			BGR24Frame bgr24Frame;
			bgr24Frame.frameData = new(std::nothrow) char[frameBytes];
			bgr24Frame.frameBytes = frameBytes;
			memcpy_s(bgr24Frame.frameData, frameBytes, frame, frameBytes);
			bgr24Frame.jpegData = new(std::nothrow) char[jpegBytes];
			bgr24Frame.jpegBytes = jpegBytes;
			memcpy_s(bgr24Frame.jpegData, jpegBytes, jpeg, jpegBytes);
			bgr24Frame.channelIndex = channel;
			bgr24Frame.NVRIp = new(std::nothrow) char[ipBytes + 1];
			bgr24Frame.NVRIp[ipBytes] = 0;
			memcpy_s(bgr24Frame.NVRIp, ipBytes, ip, ipBytes);

			BGR24FrameVect.push_back(bgr24Frame);
		}
		rwLock.unlock();
	}

	return status;
}

void BGR24FrameCache::pop_front()
{
	rwLock.lock();
	BGR24Frame bgr24Frame{ BGR24FrameVect.front() };
	delete[] bgr24Frame.frameData;
	delete[] bgr24Frame.jpegData;
	delete[] bgr24Frame.NVRIp;
	BGR24FrameVect.pop_front();
	rwLock.unlock();
}

bool BGR24FrameCache::front(BGR24Frame& frame)
{
	bool status{ false };
	frame.frameData = NULL;
	frame.frameBytes = 0;

	rwLock.lock();
	if (0 < BGR24FrameVect.size())
	{
		frame = BGR24FrameVect.front();
		status = true;
	}
	
	rwLock.unlock();

	return status;
}
