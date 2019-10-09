//
//		Copyright :					@2018, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2019-07-19
//		Description :				海康实时流类
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
	HikvisionLivestream(const long uid = -1, const unsigned short idx = -1);
	virtual ~HikvisionLivestream(void);

public:
	int openStream(void) override;
	int closeStream(void) override;
	unsigned long long capturePicture(
		const char* data = NULL, const unsigned long long dataBytes = 0) override;

protected:
	unsigned long long captureJPEGPicture(
		const char* data = NULL, const unsigned long long dataBytes = 0) override;

private:
	static void CALLBACK livestreamDataCaptureCallback(
		long livestreamID = 0, unsigned long dataType = 0, 
		unsigned char* streamData = NULL, unsigned long dataBytes = 0, void* ctx = NULL);

private:
	const long loginUserID;
};//class HikvisionLivestream

NS_END

#endif//HIKVISION_LIVE_STREAM_H
