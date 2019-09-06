#include "error.h"
#include "BGR24FrameList.h"

BGR24FrameList::BGR24FrameList(const int capacity /* = 25 */) : FIFOList(capacity)
{}

BGR24FrameList::~BGR24FrameList()
{}

int BGR24FrameList::addFrame(
	const int channel /* = -1  */, const char* ip /* = NULL  */, const int ipBytes /* = 0 */, 
	const char* frame /* = NULL */, const int frameBytes /* = 0 */, 
	const char* jpeg /* = NULL */, const int jpegBytes /* = 0 */)
{
	int status{ ERR_INVALID_PARAM };

	if (-1 < channel && ip && 0 < ipBytes && frame && 0 < frameBytes && jpeg && 0 < jpegBytes)
	{
		BGR24Frame* elem{ NULL };

		try
		{
			elem = new BGR24Frame;
			elem->frameData = new char[frameBytes];
			elem->frameBytes = frameBytes;
			memcpy_s(elem->frameData, frameBytes, frame, frameBytes);
			elem->jpegData = new char[jpegBytes];
			elem->jpegBytes = jpegBytes;
			memcpy_s(elem->jpegData, jpegBytes, jpeg, jpegBytes);
			elem->channelIndex = channel;
			elem->NVRIp = new char[ipBytes + 1];
			elem->NVRIp[ipBytes] = 0;
			memcpy_s(bgr24Frame.NVRIp, ipBytes, ip, ipBytes);

			status = FIFOList::pushBack(elem);
		}
		catch (std::exception* e)
		{
			status = ERR_BAD_ALLOC;
			if (elem)
			{
				boost::checked_array_delete(elem->NVRIp);
				boost::checked_array_delete(elem->jpegData);
				boost::checked_array_delete(elem->frameData);
				boost::checked_delete(elem);
			}
		}
	}

	return status;
}

void BGR24FrameList::removeFrame()
{
	BGR24Frame* elem{ getNextFrame() };
	boost::checked_array_delete(elem->NVRIp);
	boost::checked_array_delete(elem->jpegData);
	boost::checked_array_delete(elem->frameData);
	boost::checked_delete(elem);
	FIFOList::popFront();
}

BGR24Frame* BGR24FrameList::getNextFrame()
{
	return reinterpret_cast<BGR24Frame*>(FIFOList::getFrontData());
}
