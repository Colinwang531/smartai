//
//		Copyright :					@2018, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2019-07-19
//		Description :				����ʵʱ���������
//
//		History:						Author										Date													Description
//											������										2019-07-22									����
//

#ifndef HIKVISION_LIVE_STREAM_H
#define HIKVISION_LIVE_STREAM_H

#include "Stream/Livestream.h"

NS_BEGIN(stream, 1)

class HikvisionLivestream : public Livestream
{
public:
	HikvisionLivestream(void);
	virtual ~HikvisionLivestream(void);

public:
	int open(const int userID = -1, const int streamNo = -1) override;
	int close(void) override;

private:
	static void __stdcall livestreamDataCaptureCallback(
		long livesteamID = 0, unsigned long dataType = 0, 
		unsigned char* streamData = NULL, unsigned long dataBytes = 0, void* ctx = NULL);

protected:
	int channelIndex;
};//class HikvisionLivestream

NS_END

#endif//HIKVISION_LIVE_STREAM_H
