#ifndef BGR24_FRAME_CACHE_H
#define BGR24_FRAME_CACHE_H

#include <list>
#include <mutex>
#include "predef.h"

typedef struct tagBGR24Frame_t
{
	char* frameData;
	int frameBytes;
	char* jpegData;
	int jpegBytes;
	int channelIndex;
	char* NVRIp;
}BGR24Frame;

class BGR24FrameCache
{
public:
	BGR24FrameCache(const int frames = 3);
	~BGR24FrameCache();

	int insert(
		const int channel = -1, const char* ip = NULL, const int ipBytes = 0,
		const char* frame = NULL, const int frameBytes = 0,
		const char* jpeg = NULL, const int jpegBytes = 0);
	void pop_front();
	bool front(BGR24Frame& frame);

private:
	const int maxFrameNumber;
	std::list<BGR24Frame> BGR24FrameVect;
	std::mutex rwLock;
};//class BGR24FrameCache

#endif//BGR24_FRAME_CACHE_H

