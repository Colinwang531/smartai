//
//		Copyright :					@2018, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2019-07-19
//		Description :				海康实时流抽象基类
//
//		History:						Author										Date													Description
//											王科威										2019-07-22									创建
//

#ifndef HIKVISION_LIVE_STREAM_H
#define HIKVISION_LIVE_STREAM_H

#include "Stream/Livestream.h"

NS_BEGIN(stream, 1)

class HikvisionLivestream : public Livestream
{
public:
	HikvisionLivestream(const unsigned short idx = 0);
	virtual ~HikvisionLivestream(void);

public:
	int open(const int userID = -1) override;
	int close(void) override;
	int capture(
		const int userID, const int cameraIndex, char*& jpegData, const int jpegBytes = 1024 * 1024) override;

private:
	static void /*CALLBACK*/__stdcall livestreamDataCaptureCallback(
		long livesteamID = 0, unsigned long dataType = 0, 
		unsigned char* streamData = NULL, unsigned long dataBytes = 0, void* ctx = NULL);
};//class HikvisionLivestream

NS_END

#endif//HIKVISION_LIVE_STREAM_H
