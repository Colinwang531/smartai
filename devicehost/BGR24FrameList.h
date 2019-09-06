#ifndef BGR24_FRAME_LIST_H
#define BGR24_FRAME_LIST_H

#include "DataStruct/FIFOList.h"
using FIFOList = NS(datastruct, 1)::FIFOList;

class BGR24FrameList : protected FIFOList
{
public:
	BGR24FrameList(const int capacity = 25);
	~BGR24FrameList(void);

public:
	int addFrame(
		const int channel = -1 , const char* ip = NULL , const int ipBytes = 0,
		const char* frame = NULL, const int frameBytes = 0, 
		const char* jpeg = NULL, const int jpegBytes = 0);
	void removeFrame(void);
	BGR24Frame* getNextFrame(void)
};//class BGR24FrameCache

#endif//BGR24_FRAME_LIST_H

