//
//		Copyright :					@2018, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2019-07-19
//		Description :				ʵʱ����
//
//		History:						Author										Date													Description
//											������										2019-07-22									����
//

#ifndef LIVE_STREAM_H
#define LIVE_STREAM_H

#include <vector>
#include "AVStream.h"

NS_BEGIN(stream, 1)

class Livestream : protected AVStream
{
public:
	Livestream(const unsigned short idx = -1);
	virtual ~Livestream(void);

public:
	virtual int open(const int = -1) = 0;
	virtual int close(void) = 0;
	//Return : Bytes of JPEG data.
	virtual int capture(
		const int userID, const int cameraIndex, char*& jpegData, const int jpegBytes = 1024 * 1024) = 0;
	virtual void queue(const int type, std::vector<void*>& out) = 0;

protected:
	virtual void captureVideoDataNotifiy(
		const unsigned char* streamData = NULL, const long dataBytes = 0, const long dataType = -1) = 0;
// 	virtual void audioDataCaptureNotifier(const char* data = NULL, const int dataBytes = 0) = 0;

protected:
	const unsigned short cameraIndex;
	long livestreamID;
};//class Livestream

NS_END

#endif//LIVE_STREAM_H
